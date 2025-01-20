#include "network.hpp"

#include <curl/curl.h>
#include <spdlog/spdlog.h>

#include <nlohmann/json.hpp>
#include <string>

namespace Vapp {

Network::Network() {
    spdlog::debug("Constructor Network");
}

Network::~Network() {
    spdlog::debug("Destructor Network");
}

ApiResponse Network::apiRequest(const ApiRequest request) {
    CURL* curl = curl_easy_init();
    ApiResponse response;

    if (curl) {
        struct curl_slist* headers = nullptr;
        if (!request.apiKey.empty()) {
            std::string authHeader = "Authorization: Bearer " + request.apiKey;
            headers = curl_slist_append(headers, authHeader.c_str());
        }
        headers = curl_slist_append(headers, "Content-Type: application/json");
        for (const auto& header : request.headers) {
            headers = curl_slist_append(headers, header.c_str());
        }

        // setup cURL options
        curl_easy_setopt(curl, CURLOPT_URL, request.url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.raw);

        if (request.method == "POST") {
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            if (!request.body.empty()) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.body.c_str());
            }
        } else if (request.method == "PUT") {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
            if (!request.body.empty()) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.body.c_str());
            }
        } else if (request.method == "DELETE") {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        } else if (request.method == "GET") {
            // nothing for now?
        } else {
            return response;
        }

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            spdlog::warn("cURL error: {}", curl_easy_strerror(res));
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    } else {
        spdlog::warn("Failed to initialize cURL.");
    }

    if (!response.raw.empty()) {
        response.asJson = nlohmann::json::parse(response.raw);
    }

    return response;
}

size_t Network::writeCallback(void* contents, size_t size, size_t nmemb, std::string* userData) {
    size_t totalSize = size * nmemb;
    userData->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

}  // namespace Vapp
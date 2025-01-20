#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

namespace Vapp {

struct ApiRequest {
    std::string url;
    std::string apiKey;
    std::string body;
    std::vector<std::string> headers;
    std::string method = "GET";  
};

struct ApiResponse {
    std::string raw;
    nlohmann::json asJson;
};

class Network {
   public:
    Network();
    ~Network();
    ApiResponse apiRequest(const ApiRequest request);
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* userData);

   private:
};

}  // namespace Vapp
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include "vapp/core/network.hpp"

class NetworkTest : public ::testing::Test {
   protected:
    Vapp::Network network;
};

TEST_F(NetworkTest, Constructor) {
    EXPECT_NO_THROW(Vapp::Network());
}

TEST_F(NetworkTest, ApiRequestGet) {
    Vapp::ApiRequest request;
    request.url = "https://jsonplaceholder.typicode.com/posts/1";
    request.method = "GET";

    auto response = network.apiRequest(request);
    EXPECT_FALSE(response.raw.empty());
    EXPECT_NO_THROW(response.asJson.at("id"));
}

TEST_F(NetworkTest, ApiRequestPost) {
    Vapp::ApiRequest request;
    request.url = "https://jsonplaceholder.typicode.com/posts";
    request.method = "POST";
    request.body = R"({"title": "foo", "body": "bar", "userId": 1})";

    auto response = network.apiRequest(request);
    EXPECT_FALSE(response.raw.empty());
    EXPECT_NO_THROW(response.asJson.at("id"));
}

TEST_F(NetworkTest, ApiRequestInvalidUrl) {
    Vapp::ApiRequest request;
    request.url = "invalid_url";
    request.method = "GET";

    auto response = network.apiRequest(request);
    EXPECT_TRUE(response.raw.empty());
}

TEST_F(NetworkTest, ApiRequestInvalidMethod) {
    Vapp::ApiRequest request;
    request.url = "https://jsonplaceholder.typicode.com/posts/1";
    request.method = "INVALID_METHOD";

    auto response = network.apiRequest(request);
    EXPECT_TRUE(response.raw.empty());
}

TEST_F(NetworkTest, ApiRequestWithHeaders) {
    Vapp::ApiRequest request;
    request.url = "https://jsonplaceholder.typicode.com/posts/1";
    request.method = "GET";
    request.headers.push_back("Custom-Header: CustomValue");

    auto response = network.apiRequest(request);
    EXPECT_FALSE(response.raw.empty());
    EXPECT_NO_THROW(response.asJson.at("id"));
}
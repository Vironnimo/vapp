#include "ai_request.hpp"

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "vapp/core/network.hpp"

namespace Vapp {

AiRequest::AiRequest() {
}

AiRequest::~AiRequest() {
}

void AiRequest::init(std::shared_ptr<Network> network) {
    m_network = std::move(network);
}

// chatgpt request
// auto gptKey = ;
// Vapp::ApiRequest chatGptRequest;
// chatGptRequest.url = "https://api.openai.com/v1/responses";
// chatGptRequest.method = "POST";
// // chatGptRequest.method = Vapp::ApiRequest::POST;
// chatGptRequest.apiKey = gptKey;
// chatGptRequest.body = R"({
//     "model": "gpt-3.5-turbo",
//     "input": "Hello! How are you?"
// })";
// auto chatGptResponse = m_vapp->network()->apiRequest(chatGptRequest);
// spdlog::info("ChatGPT Response (700 chars):\n{}", chatGptResponse.raw.substr(0, 700));
// spdlog::info("ChatGPT Response (JSON):\n{}", chatGptResponse.asJson.dump(4));

AiCommand AiRequest::requestCommand(const std::string& promptMessage) {
    // hugging face request
    const auto* const huggingFaceKey = "";
    ApiRequest huggingFaceRequest;
    huggingFaceRequest.url = "https://router.huggingface.co/novita/v3/openai/chat/completions";
    huggingFaceRequest.method = "POST";
    huggingFaceRequest.apiKey = huggingFaceKey;

    std::string model = "deepseek/deepseek-v3-0324";
    std::string systemPrompt =
        "You are a JSON command generator. only output raw JSON. Respond with the command to execute and the args to the command.";
    std::string body = "{\"messages\": [{\"role\": \"system\", \"content\": \"" + systemPrompt +
                       "\"}, {\"role\": \"user\", \"content\": \"" + promptMessage + "\"}], \"model\": \"" + model +
                       "\", \"stream\": false}";
    huggingFaceRequest.body = body;

    auto huggingFaceResponse = m_network->apiRequest(huggingFaceRequest);
    auto message = huggingFaceResponse.asJson["choices"][0]["message"]["content"].get<std::string>();
    if (message.rfind("```", 0) == 0) {
        message.erase(0, 3);
        if (auto p = message.rfind("```"); p != std::string::npos) {
            message.erase(p);
        }
    }
    if (auto pos = message.find('{'); pos != std::string::npos) {
        message.erase(0, pos);
    }
    spdlog::info("Hugging Face Response (message):\n{}", message);
    try {
        auto cmd = nlohmann::json::parse(message);
        auto cmdName = cmd.at("command").get<std::string>();
        spdlog::info("Command: {}", cmdName);
        auto args = cmd.at("args");
        if (!args.is_array() || args.empty()) {
            spdlog::warn("No arguments provided in AI response.");
            return AiCommand{cmdName, ""};
        }
        auto arg1 = args.at(0).get<std::string>();
        spdlog::info("Args: {}", arg1);

        AiCommand aiCommand;
        aiCommand.command = cmdName;
        aiCommand.args = arg1;
        return aiCommand;
    } catch (const std::exception& e) {
        spdlog::error("Failed to parse AI JSON response: {}", e.what());
    }

    return AiCommand();
    // spdlog::info("Hugging Face Response (message):\n{}", message);
    // spdlog::info("Hugging Face Response (JSON):\n{}", huggingFaceResponse.asJson.dump(4));
}

}  // namespace Vapp
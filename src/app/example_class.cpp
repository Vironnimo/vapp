#include "example_class.hpp"

#include <fstream>
#include <spdlog/spdlog.h>
#include <vosk/vosk_api.h>

#include "vapp/vapp.hpp"

void ExampleClass::databaseExamples() {
    m_vapp->database()->exec("DROP TABLE IF EXISTS test");
    m_vapp->database()->exec("CREATE TABLE test (id INTEGER PRIMARY KEY, value TEXT)");

    m_vapp->database()->exec("INSERT INTO test VALUES (NULL, \"test\")");
    m_vapp->database()->exec("INSERT INTO test VALUES (NULL, \"test2\")");
    m_vapp->database()->exec("INSERT INTO test VALUES (NULL, \"test3\")");

    auto query = m_vapp->database()->query("SELECT * FROM test");
    while (query.executeStep()) {
        int id = query.getColumn(0);
        std::string text = query.getColumn(1);

        spdlog::info("DB Test - id: {}, text: \"{}\"", id, text);
    }
}

ExampleClass::ExampleClass(std::shared_ptr<Vapp::Vapp> vapp) : m_vapp(std::move(vapp)) {
    spdlog::debug("Constructor ExampleClass");

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

    // hugging face request
    // start timer for api request
    // auto huggingFaceKey = ;
    // m_vapp->timer()->start("hugging_face_request");
    // Vapp::ApiRequest huggingFaceRequest;
    // huggingFaceRequest.url = "https://router.huggingface.co/novita/v3/openai/chat/completions";
    // huggingFaceRequest.method = "POST";
    // huggingFaceRequest.apiKey = huggingFaceKey;
    // huggingFaceRequest.body = R"({
    // "messages": [
    //     {
    //         "role": "system",
    //         "content":"You are a JSON‐only command generator.  You *must* output *only* valid JSON text.  Every key and
    //         string value must be enclosed in double quotes (\\\"key\\\": \\\"value\\\").  Do NOT output anything else—not
    //         even formatting or code fences!"
    //     },
    //     {
    //         "role": "user",
    //         "content": "Create a folder at C:/temp/my_new_folder"
    //     }
    // ],
    // "model": "deepseek/deepseek-v3-0324",
    // "stream": false
    // })";
    // auto huggingFaceResponse = m_vapp->network()->apiRequest(huggingFaceRequest);
    // auto elapsed = m_vapp->timer()->end("hugging_face_request");
    // spdlog::info("Request took {} ms", elapsed);
    // auto message = huggingFaceResponse.asJson["choices"][0]["message"]["content"].get<std::string>();
    // if (message.rfind("```", 0) == 0) {
    //     message.erase(0, 3);
    //     if (auto p = message.rfind("```"); p != std::string::npos) {
    //         message.erase(p);
    //     }
    // }
    // if (auto pos = message.find('{'); pos != std::string::npos) {
    //     message.erase(0, pos);
    // }
    // spdlog::info("Hugging Face Response (message):\n{}", message);
    // try {
    //     auto cmd = nlohmann::json::parse(message);
    //     auto name = cmd.at("command").get<std::string>();
    //     spdlog::info("Command: {}", name);
    //     auto args = cmd.at("args");
    //     auto path = args.at(0).get<std::string>();
    //     // spdlog::info("Args: {}", args.dump(4));
    //     spdlog::info("Path: {}", path);
    // } catch (const std::exception& e) {
    //     spdlog::error("Failed to parse AI JSON response: {}", e.what());
    // }
    // spdlog::info("Hugging Face Response (message):\n{}", message.dump(4));
    // spdlog::info("Hugging Face Response (700 chars):\n{}", huggingFaceResponse.raw.substr(0, 700));
    // spdlog::info("Hugging Face Response (JSON):\n{}", huggingFaceResponse.asJson.dump(4));

    databaseExamples();

    m_vapp->events()->emit("example_event");

    m_vapp->actions()->add("app.example", "Example Action", [this]() {
        spdlog::info("Action from ExampleClass: member variable value: {}", m_testValue);
        m_testValue++;
    });
    m_vapp->actions()->addKeybinding("app.example", {GLFW_KEY_S, false, false, false});
    m_vapp->actions()->add("app.event_test", "Action: Example Action with Event trigger", [this]() {
        m_vapp->events()->emit("example_event", &m_testValue);
    });
    m_vapp->actions()->addKeybinding("app.event_test", {GLFW_KEY_F, false, false, false});
}

ExampleClass::~ExampleClass() {
    spdlog::debug("Destructor ExampleClass");
}

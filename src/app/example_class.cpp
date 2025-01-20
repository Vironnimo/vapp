#include "example_class.hpp"

#include <spdlog/spdlog.h>

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

    Vapp::ApiRequest request;
    request.url = "https://jsonplaceholder.typicode.com/posts";
    auto response = m_vapp->network()->apiRequest(request);
    spdlog::info("Response (700 chars):\n{}", response.raw.substr(0, 700));
    spdlog::info("First entry:");
    for (const auto& entry : response.asJson) {
        auto id = entry["id"].get<int>();
        auto userId = entry["userId"].get<int>();
        auto title = entry["title"].get<std::string>();
        auto body = entry["body"].get<std::string>();

        spdlog::info("ID: {}\nUserID: {}\nTitle: {}\nBody: {}", id, userId, title, body);
        break;
    }
    auto first = response.asJson[0];
    spdlog::info("TEEEEEEEST:::  {}", first["id"].get<int>());

    databaseExamples();

    m_vapp->events()->emit("example_event");

    m_vapp->actions()->add("app.example", "Example Action", [this]() {
        spdlog::info("Action from ExampleClass: member variable value: {}", m_testValue);
        m_testValue++;
    });
    m_vapp->actions()->addKeybinding("app.example", {GLFW_KEY_S, false, false, false});
    m_vapp->actions()->add("app.event_test", "Action: Example Action with Event trigger",
                           [this]() { m_vapp->events()->emit("example_event", &m_testValue); });
    m_vapp->actions()->addKeybinding("app.event_test", {GLFW_KEY_F, false, false, false});
}

ExampleClass::~ExampleClass() {
    spdlog::debug("Destructor ExampleClass");
}



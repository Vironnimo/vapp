#include "app.hpp"

#include <imgui.h>
#include <spdlog/spdlog.h>

#include <memory>

#include "example_class.hpp"
#include "example_fragment.hpp"
#include "vapp/core/app_params.hpp"
#include "vapp/core/event_system.hpp"
#include "vapp/vapp.hpp"

void App::testStuff() {
    m_vapp->getEventSystem()->subscribe("example_event", [this](void* data) {
        spdlog::info("test event here. app class value: {}", m_appValue);

        // need to dereference the value to use it
        if (data != nullptr) {
            int* value = static_cast<int*>(data);
            m_appValue = m_appValue + *value;
        }
    });

    m_exampleClass = new ExampleClass(m_vapp);
    m_vapp->getActions()->execute("app.example");
    m_vapp->getActions()->execute("app.example");

    m_vapp->attachFragment(std::make_unique<TestFragment>());
}

App::App() {
    init();
}

void App::run() {
    testStuff();

    m_vapp->run();
}

void App::init() {
    // setup Vapp
    Vapp::AppParams appParams;
    // create menucallback if a menubar is wanted
    appParams.menuCallback = [this]() {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New", "Ctrl+N")) {
            }
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Quit", "Alt+F4")) {
                m_vapp->getActions()->execute("app.quit");
                spdlog::debug("Quit clicked");
            }
            ImGui::EndMenu();
        }
    };
    m_vapp = std::make_shared<Vapp::Vapp>(appParams);
}

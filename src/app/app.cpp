#include "app.hpp"

#include <imgui.h>
#include <spdlog/spdlog.h>

#include <memory>

#include "example_class.hpp"
#include "example_fragment.hpp"
#include "vapp/core/app_params.hpp"
#include "vapp/core/event_system.hpp"
#include "vapp/core/resource_manager.hpp"
#include "vapp/vapp.hpp"

void App::testStuff() {
    m_vapp->events()->subscribe("example_event", [this](void* data) {
        spdlog::info("test event here. app class value: {}", m_appValue);

        // need to dereference the value to use it
        if (data != nullptr) {
            int* value = static_cast<int*>(data);
            m_appValue = m_appValue + *value;
        }
    });

    m_vapp->actions()->execute("app.example");
    m_vapp->actions()->execute("app.example");

    m_vapp->attachFragment(std::make_unique<TestFragment>());
}

App::App() {
    spdlog::debug("Constructor App");
    init();
    loadResources();
    testStuff();
}

App::~App() {
    spdlog::debug("Destructor App");
}

void App::loadResources() {
    m_vapp->timer()->start("resources");
    auto rm = m_vapp->resources();

    rm->load<Vapp::Image>("snake", "snake.png");

    auto get = m_vapp->timer()->get("resources");

    rm->load<Vapp::Sound>("click", "click.wav");

    auto end = m_vapp->timer()->end("resources");
    spdlog::info("Timer - load Resources: {} ms", end);
}

void App::run() {
    m_vapp->run();
}

void App::init() {
    // setup Vapp
    Vapp::AppParams appParams;
    appParams.maxFps = 60;
    // appParams.useFileLogger = true;
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
                spdlog::info("Menu: Quit clicked");
                m_vapp->actions()->execute("app.quit");
            }
            ImGui::EndMenu();
        }
    };
    m_vapp = std::make_shared<Vapp::Vapp>(appParams);
    auto startTime = m_vapp->timer()->get("app.start");
    Vapp::log.info("Vapp Startup Time: {} ms", startTime);
    

    m_exampleClass = std::make_unique<ExampleClass>(m_vapp);
}

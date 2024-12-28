#include "app.hpp"

#include <memory>
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "example_class.hpp"
#include "example_fragment.hpp"
#include "vapp/core/app_params.hpp"
#include "vapp/vapp.hpp"

void App::testStuff() {
    m_exampleClass = new ExampleClass(m_vapp->getActions());
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
    Vapp::AppParams appParams;
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
    m_vapp = std::make_unique<Vapp::Vapp>(appParams);
}

#include "app.hpp"

#include <imgui.h>

#include "vapp/vapp.hpp"
#include "vapp/core/app_params.hpp"

App::App() {
    Vapp::AppParams appParams;
    appParams.menuCallback = setupMenu;
    Vapp::Vapp vapp(appParams);
    // Vapp::Vapp vapp;
    vapp.run();
}

void App::setupMenu() {
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New", "Ctrl+N")) {
            // Handle new
        }
        if (ImGui::MenuItem("Open", "Ctrl+O")) {
            // Handle open
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Save", "Ctrl+S")) {
            // Handle save
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Quit", "Alt+F4")) {
            // glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
        }
        ImGui::EndMenu();
    }
    
    if (ImGui::BeginMenu("Edit")) {
        if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
        if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
        ImGui::Separator();
        if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
        if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
        if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
        ImGui::EndMenu();
    }
}

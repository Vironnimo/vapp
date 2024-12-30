#include "theme.hpp"

#include <dwmapi.h>
#include <imgui.h>
#include <windows.h>

#pragma comment(lib, "dwmapi.lib")
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <IconsFontAwesome.h>
#include <spdlog/spdlog.h>

namespace Vapp {

// todo loaded fonts need to be unloaded, right?

Theme::Theme() {
    spdlog::debug("Theme Constructor");
    init();
}

void Theme::init() {
    loadFonts();
}

void Theme::pushDefaultStyle() {
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();

    style.FrameRounding = 3.0f;
    style.WindowBorderSize = 0.0f;
    style.PopupRounding = 6.0f;

    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(10.0f, 10.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(30.0f, 20.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 10.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.5f, 0.5f, 0.5f, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
}

void Theme::popDefaultStyle() {
    ImGui::PopStyleVar(4);
    ImGui::PopStyleColor(6);
}

void Theme::loadFonts() {
    ImGuiIO& io = ImGui::GetIO();

    auto* normal = io.Fonts->AddFontFromFileTTF("fonts/Inter_24pt-Regular.ttf", 20.0f);
    auto* bold = io.Fonts->AddFontFromFileTTF("fonts/Inter_24pt-Bold.ttf", 22.0f);
    auto* header1 = io.Fonts->AddFontFromFileTTF("fonts/Inter_24pt-Bold.ttf", 45.0f);
    auto* header2 = io.Fonts->AddFontFromFileTTF("fonts/Inter_24pt-Bold.ttf", 33.0f);
    auto* header3 = io.Fonts->AddFontFromFileTTF("fonts/Inter_24pt-Bold.ttf", 28.0f);

    // load font awesome
    static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};
    // static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 }; // Solid icons
    ImFontConfig config;
    config.MergeMode = true;
    config.PixelSnapH = true;
    config.GlyphMinAdvanceX = 13.0f;
    auto* awesome = io.Fonts->AddFontFromFileTTF("fonts/fa-regular-400.ttf", 28.0f, &config, icons_ranges);

    m_fonts[FontType::Normal] = normal;
    m_fonts[FontType::Bold] = bold;
    m_fonts[FontType::Header1] = header1;
    m_fonts[FontType::Header2] = header2;
    m_fonts[FontType::Header3] = header3;
    m_fonts[FontType::Awesome] = awesome;

    io.FontDefault = m_fonts[FontType::Normal];
}

void Theme::pushFont(const FontType type) {
    ImGui::PushFont(m_fonts[type]);
}

void Theme::popFont() {
    ImGui::PopFont();
}

void Theme::enableDarkModeForWindow(GLFWwindow* window) {
    BOOL useDarkMode = TRUE;
    HWND hwnd = glfwGetWin32Window(window);
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof(useDarkMode));
}

Theme::~Theme() {
    spdlog::debug("Theme Destructor");
    // ImGui::GetIO().Fonts->Clear();
    m_fonts.clear();
}

}  // namespace Vapp
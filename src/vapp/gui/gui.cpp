#include "gui.hpp"

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <utility>

#include "vapp/gui/theme.hpp"
#include "vapp/core/app_settings.hpp"
#include "vapp/core/resource_manager.hpp"
#include "vapp/vapp.hpp"

namespace Vapp {

Gui::Gui(AppSettings params, Vapp* vapp) 
    : m_settings(std::move(params)), m_vapp(vapp) {
    spdlog::debug("Constructor Gui");
    init();
}

Gui::~Gui() {
    spdlog::debug("Destructor Gui");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Gui::init() {
    initGlfw();
    m_window = createWindow(m_settings);
    initImGui();

    // style stuff
    m_style = std::make_shared<Theme>();
    m_style->enableDarkModeForWindow(m_window);
}

bool Gui::initGlfw() {
    glfwSetErrorCallback(glfwErrorCallback);
    if (glfwInit() == GLFW_FALSE) {
        spdlog::error("Failed to initialize GLFW3");
        return false;
    }

    return true;
}

void Gui::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // enable keyboard controls

    // init opengl for imgui
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init();
}

void Gui::render() {
    if (glfwWindowShouldClose(m_window) != 0) {
        windowShouldClose = true;
    }
    if (windowShouldClose) {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
        return;
    }

    glfwPollEvents();
    startFrame();
    mainWindowBegin();

    // render menu, if available
    // make extra style for menu
    if (m_settings.menuCallback) {
        if (ImGui::BeginMenuBar()) {
            m_settings.menuCallback();

            ImGui::EndMenuBar();
        }
    }

    m_style->pushDefaultStyle();

    for (const auto& item : m_fragments) {
        item->draw();
    }

    m_style->popDefaultStyle();
    mainWindowEnd();
    endFrame();
}

void Gui::mainWindowBegin() const {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;

    if (m_settings.menuCallback) {
        flags |= ImGuiWindowFlags_MenuBar;
    }
    ImGui::Begin("##main_window", nullptr, flags);
}

void Gui::mainWindowEnd() {
    ImGui::End();
}

void Gui::startFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Gui::endFrame() {
    ImGui::Render();
    int display_w = 0;
    int display_h = 0;
    glfwGetFramebufferSize(m_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_window);
}

void Gui::attachFragment(std::unique_ptr<IFragment> fragment) {
    fragment->setStyle(m_style);
    fragment->setVapp(m_vapp);
    m_fragments.push_back(std::move(fragment));
}

GLFWwindow* Gui::createWindow(AppSettings& params) {
    auto* window = glfwCreateWindow(params.windowWidth, params.windowHeight, params.windowTitle.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        spdlog::error("Failed to create GLFW window");
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowSizeLimits(window, params.windowWidthMin, params.windowHeightMin, GLFW_DONT_CARE, GLFW_DONT_CARE);
    if (params.windowCentered) {
        centerWindow(params, window);
    }

    return window;
}

void Gui::centerWindow(AppSettings& params, GLFWwindow* window) {
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    if (primaryMonitor != nullptr) {
        const GLFWvidmode* vidMode = glfwGetVideoMode(primaryMonitor);

        const int windowPosX = vidMode->width / 2 - params.windowWidth / 2;
        const int windowPosY = vidMode->height / 2 - params.windowHeight / 2;

        glfwSetWindowPos(window, windowPosX, windowPosY);
    }
}

GLFWwindow* Gui::getWindow() const {
    return m_window;
}

void Gui::glfwErrorCallback(int error, const char* description) {
    // fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    spdlog::error("GLFW Error {}: {}", error, description);
}

}  // namespace Vapp
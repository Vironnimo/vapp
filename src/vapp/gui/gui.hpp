#pragma once

#include <imgui_impl_glfw.h>

#include <list>
#include <string>

#include "vapp/core/app_params.hpp"
#include "vapp/core/resource_manager.hpp"

#include "i_fragment.hpp"
#include "theme.hpp"

namespace Vapp {

class Gui {
   public:
    bool windowShouldClose = false;

   public:
    Gui(AppParams params, std::shared_ptr<ResourceManager> resourceManager);
    ~Gui();
    void init();

    void render();
    void attachFragment(std::unique_ptr<IFragment> fragment);

    GLFWwindow* getWindow() const;
    static void glfwErrorCallback(int error, const char* description);

   private:
    void startFrame();
    void endFrame();

    bool initGlfw();
    void initImGui();

    void mainWindowBegin() const;
    void mainWindowEnd();

    GLFWwindow* createWindow(AppParams& params);
    void centerWindow(AppParams& params, GLFWwindow* window);

   private:
    GLFWwindow* m_window = nullptr;
    AppParams m_appParams;
    ImGuiIO m_io;
    std::shared_ptr<Theme> m_style;
    std::shared_ptr<ResourceManager> m_resourceManager;
    std::list<std::unique_ptr<IFragment>> m_fragments;
};

}  // namespace Vapp

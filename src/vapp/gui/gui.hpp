#pragma once

#include <imgui_impl_glfw.h>

#include <list>
#include <string>

#include "vapp/core/app_params.hpp"
#include "fragment/i_base_fragment.hpp"
#include "gui_style.hpp"

namespace Vapp {

class Gui {
   public:
    bool windowShouldClose = false;

   public:
    Gui(AppParams params);
    ~Gui();
    void init(AppParams params);

    void render();
    void attachFragment(std::unique_ptr<IBaseFragment> fragment);

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
    std::shared_ptr<GuiStyle> m_style;
    std::list<std::unique_ptr<IBaseFragment>> m_fragments;
};

}  // namespace Vapp

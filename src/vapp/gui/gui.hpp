#pragma once

#include <imgui_impl_glfw.h>

#include <list>
#include <string>

#include "vapp/core/app_settings.hpp"
#include "vapp/core/resource_manager.hpp"

#include "i_fragment.hpp"
#include "theme.hpp"

namespace Vapp {

class Vapp;

class Gui {
   public:
    bool windowShouldClose = false;

   public:
    Gui(Vapp* vapp);
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

    GLFWwindow* createWindow(std::shared_ptr<AppSettings> settings);
    void centerWindow(std::shared_ptr<AppSettings> settings, GLFWwindow* window);

   private:
    GLFWwindow* m_window = nullptr;
    Vapp* m_vapp = nullptr; // we use a raw pointer here because we don't like weak pointers and how to use them.
    std::shared_ptr<Theme> m_style;
    std::list<std::unique_ptr<IFragment>> m_fragments;
};

}  // namespace Vapp

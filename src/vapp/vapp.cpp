#include "vapp.hpp"

#include <spdlog/common.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <utility>
#include <vector>

#include "vapp/gui/gui.hpp"
#include "vapp/core/actions.hpp"
#include "vapp/core/app_params.hpp"
#include "vapp/core/event_system.hpp"
#include "vapp/core/timer.hpp"

namespace Vapp {

Vapp::Vapp(AppParams appParams) : m_appParams(std::move(appParams)) {
    init();
    initActions();
}

void Vapp::init() {
#if defined(_DEBUG)
    spdlog::set_level(spdlog::level::debug);
    spdlog::debug("Build Configuration: Debug");
#else
    // log to file
    auto file_logger = spdlog::basic_logger_mt("file_logger", "logs.txt");
    spdlog::set_default_logger(file_logger);
#endif

    spdlog::debug("Vapp Constructor");
    m_eventSystem = std::make_shared<EventSystem>();
    m_resourceManager = std::make_shared<ResourceManager>();
    m_gui = std::make_unique<Gui>(m_appParams, m_resourceManager);
    m_timer = std::make_shared<Timer>();
}

void Vapp::initActions() {
    m_actions = std::make_shared<Actions>();

    m_actions->registerAction("app.quit", "Quit App", [this]() {
        spdlog::info("quitting..");
        m_gui->windowShouldClose = true;
    });

    m_actions->addKeybinding("app.quit", {GLFW_KEY_ESCAPE, false, false, false});
    m_actions->addKeybinding("app.quit", {GLFW_KEY_Q, false, false, false});
}

void Vapp::run() {
    spdlog::debug("run App");

    while (!m_gui->windowShouldClose) {
        m_timer->update();

        m_actions->handleInput(m_gui->getWindow());
        m_gui->render();
    }
}

std::shared_ptr<Actions> Vapp::actions() {
    return m_actions;
}

std::shared_ptr<EventSystem> Vapp::getEventSystem() {
    return m_eventSystem;
}

std::shared_ptr<ResourceManager> Vapp::getResourceManager() {
    return m_resourceManager;
}

std::shared_ptr<Timer> Vapp::timer() {
    return m_timer;
}

void Vapp::attachFragment(std::unique_ptr<IFragment> fragment) {
    m_gui->attachFragment(std::move(fragment));
}

Vapp::~Vapp() {
    spdlog::debug("Vapp Destructor");
}

}  // namespace Vapp
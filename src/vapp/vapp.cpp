#include "vapp.hpp"

#include <spdlog/common.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <utility>
#include <vector>

#include "vapp/core/actions.hpp"
#include "vapp/core/app_params.hpp"
#include "vapp/core/database.hpp"
#include "vapp/core/event_system.hpp"
#include "vapp/core/logger.hpp"
#include "vapp/core/timer.hpp"
#include "vapp/gui/gui.hpp"

namespace Vapp {

Vapp::Vapp(AppParams appParams) : m_appParams(std::move(appParams)) {
    init();
    initActions();
}

void Vapp::init() {
    // #if defined(_DEBUG)
    // spdlog::set_level(spdlog::level::debug);
    //     spdlog::debug("Build Configuration: Debug");
    // #else
    //     // log to file
    //     auto file_logger = spdlog::basic_logger_mt("file_logger", "logs.txt");
    //     spdlog::set_default_logger(file_logger);
    // #endif
    log.setLogLevel(m_appParams.logLevel);
    if (m_appParams.useFileLogger) {
        log.useFileLogger(m_appParams.loggerFile);
    }

    spdlog::debug("Constructor Vapp");
    m_timer = std::make_shared<Timer>();
    m_timer->start("app.start");
    if (m_appParams.maxFps > 0) {
        m_timer->setMaxFps(m_appParams.maxFps);
    }
    m_eventSystem = std::make_shared<EventSystem>();
    m_resourceManager = std::make_shared<ResourceManager>();
    m_gui = std::make_unique<Gui>(m_appParams, m_resourceManager);
    m_db = std::make_shared<Database>("vapp.db3");
}

void Vapp::initActions() {
    m_actions = std::make_shared<Actions>();

    m_actions->add("app.quit", "Quit App", [this]() {
        spdlog::info("Action from Vapp: App quitting..");
        m_gui->windowShouldClose = true;
    });

    m_actions->addKeybinding("app.quit", {GLFW_KEY_ESCAPE, false, false, false});
    m_actions->addKeybinding("app.quit", {GLFW_KEY_Q, false, false, false});
}

void Vapp::run() {
    spdlog::info("Vapp: run");

    while (!m_gui->windowShouldClose) {
        m_timer->update();
        // spdlog::debug("FPS: {}", m_timer->getFps());

        m_actions->handleInput(m_gui->getWindow());
        m_gui->render();
    }
}

std::shared_ptr<Actions> Vapp::actions() {
    return m_actions;
}

std::shared_ptr<EventSystem> Vapp::events() {
    return m_eventSystem;
}

std::shared_ptr<ResourceManager> Vapp::resources() {
    return m_resourceManager;
}

std::shared_ptr<Timer> Vapp::timer() {
    return m_timer;
}

std::shared_ptr<Database> Vapp::database() {
    return m_db;
}

void Vapp::attachFragment(std::unique_ptr<IFragment> fragment) {
    m_gui->attachFragment(std::move(fragment));
}

Vapp::~Vapp() {
    spdlog::debug("Destructor Vapp");
}

}  // namespace Vapp
#include "vapp.hpp"

#include <spdlog/common.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <utility>
#include <vector>

#include "gui/gui.hpp"
#include "vapp/core/app_params.hpp"
#include "vapp/core/actions.hpp"

namespace Vapp {

Vapp::Vapp(AppParams appParams) : m_appParams(std::move(appParams)) {
    init();
    initActions();
    spdlog::debug("Vapp Constructor");
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

    m_gui = std::make_unique<Gui>(m_appParams);
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

std::shared_ptr<Actions> Vapp::getActions() {
    return m_actions;
}

void Vapp::attachFragment(std::unique_ptr<IBaseFragment> fragment) {
    m_gui->attachFragment(std::move(fragment));
}

void Vapp::run() {
    spdlog::debug("run App");

    while (!m_gui->windowShouldClose) {
        m_actions->handleInput(m_gui->getWindow());
        m_gui->render();
    }
}

Vapp::~Vapp() {
    spdlog::debug("Vapp Destructor");
}

}  // namespace Vapp
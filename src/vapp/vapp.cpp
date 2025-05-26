#include "vapp.hpp"

#include <spdlog/common.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <utility>
#include <vector>

#include "vapp/core/actions.hpp"
#include "vapp/core/app_settings.hpp"
#include "vapp/core/database.hpp"
#include "vapp/core/event_system.hpp"
#include "vapp/core/timer.hpp"
#include "vapp/gui/gui.hpp"
#include "vapp/core/network.hpp"
#include "vapp/core/audio_recorder.hpp"
#include "vapp/core/speech_to_text.hpp"
#include "vapp/core/ai_request.hpp"

namespace Vapp {

Vapp::Vapp(AppSettings settings) : m_settings(std::make_shared<AppSettings>(std::move(settings))) {
    init();
}

void Vapp::init() {
    spdlog::debug("Constructor Vapp");

    m_timer = std::make_shared<Timer>();
    m_timer->start("app.start");
    if (m_settings->maxFps > 0) {
        m_timer->setMaxFps(m_settings->maxFps);
    }
    m_eventSystem = std::make_shared<EventSystem>();
    m_resourceManager = std::make_shared<ResourceManager>();
    m_db = std::make_shared<Database>("vapp.db3");
    m_actions = std::make_shared<Actions>();
    initActions();
    m_gui = std::make_unique<Gui>(this);
    m_network = std::make_unique<Network>(); 
    m_audioRecorder = std::make_shared<AudioRecorder>();
    m_audioRecorder->initialize();
    m_audioRecorder->setAmplification(5.0f);  
    m_speechToText = std::make_shared<SpeechToText>();
    m_speechToText->init();
    m_aiRequest = std::make_shared<AiRequest>();
    m_aiRequest->init(m_network);

    // testing
}

void Vapp::initActions() {
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

std::shared_ptr<AppSettings> Vapp::settings() {
    return m_settings;
}

std::shared_ptr<Network> Vapp::network() {
    return m_network;
}

std::shared_ptr<AudioRecorder> Vapp::audioRecorder() {
    return m_audioRecorder;
}

std::shared_ptr<SpeechToText> Vapp::speechToText() {
    return m_speechToText;
}

std::shared_ptr<AiRequest> Vapp::aiRequest() {
    return m_aiRequest;
}

void Vapp::attachFragment(std::unique_ptr<IFragment> fragment) {
    m_gui->attachFragment(std::move(fragment));
}

Vapp::~Vapp() {
    spdlog::debug("Destructor Vapp");
}

}  // namespace Vapp
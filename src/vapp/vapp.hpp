#pragma once

#include <memory>

#include "core/ai_request.hpp"
#include "core/speech_to_text.hpp"
#include "vapp/core/app_settings.hpp"
#include "vapp/core/actions.hpp"
#include "vapp/gui/gui.hpp"
#include "vapp/gui/i_fragment.hpp"
#include "vapp/core/event_system.hpp"
#include "vapp/core/resource_manager.hpp"
#include "vapp/core/timer.hpp"
#include "vapp/core/database.hpp"
#include "vapp/core/network.hpp"
#include "vapp/core/audio_recorder.hpp"
#include "vapp/core/ai_request.hpp"

namespace Vapp {

class Vapp {
   public:
    Vapp(AppSettings settings = AppSettings());
    ~Vapp();
    void init();
    void initActions();

    std::shared_ptr<Actions> actions();
    std::shared_ptr<EventSystem> events();
    std::shared_ptr<ResourceManager> resources();
    std::shared_ptr<Timer> timer();
    std::shared_ptr<Database> database();
    std::shared_ptr<AppSettings> settings();
    std::shared_ptr<Network> network();
    std::shared_ptr<AudioRecorder> audioRecorder();
    std::shared_ptr<SpeechToText> speechToText();
    std::shared_ptr<AiRequest> aiRequest();

    void attachFragment(std::unique_ptr<IFragment> fragment);

    void run();

   private:
    std::unique_ptr<Gui> m_gui;
    std::shared_ptr<AppSettings> m_settings;
    std::shared_ptr<Actions> m_actions;
    std::shared_ptr<Database> m_db;
    std::shared_ptr<EventSystem> m_eventSystem;
    std::shared_ptr<ResourceManager> m_resourceManager;
    std::shared_ptr<Timer> m_timer;
    std::shared_ptr<Network> m_network;
    std::shared_ptr<AudioRecorder> m_audioRecorder;
    std::shared_ptr<SpeechToText> m_speechToText; 
    std::shared_ptr<AiRequest> m_aiRequest;
};

}  // namespace Vapp
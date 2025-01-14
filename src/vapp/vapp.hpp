#pragma once

#include <memory>

#include "vapp/core/app_settings.hpp"
#include "vapp/core/actions.hpp"
#include "vapp/gui/gui.hpp"
#include "vapp/gui/i_fragment.hpp"
#include "vapp/core/event_system.hpp"
#include "vapp/core/resource_manager.hpp"
#include "vapp/core/timer.hpp"
#include "vapp/core/database.hpp"

namespace Vapp {

class Vapp : public std::enable_shared_from_this<Vapp> {
   public:
    Vapp(AppSettings appParams = AppSettings());
    ~Vapp();
    void init();
    void initActions();

    std::shared_ptr<Actions> actions();
    std::shared_ptr<EventSystem> events();
    std::shared_ptr<ResourceManager> resources();
    std::shared_ptr<Timer> timer();
    std::shared_ptr<Database> database();

    void attachFragment(std::unique_ptr<IFragment> fragment);

    void run();

   private:
    AppSettings m_settings;
    std::shared_ptr<Actions> m_actions;
    std::unique_ptr<Gui> m_gui;
    std::shared_ptr<Database> m_db;
    std::shared_ptr<EventSystem> m_eventSystem;
    std::shared_ptr<ResourceManager> m_resourceManager;
    std::shared_ptr<Timer> m_timer;
};

}  // namespace Vapp
#pragma once

#include <memory>

#include "vapp/core/app_params.hpp"
#include "vapp/core/actions.hpp"
#include "vapp/gui/gui.hpp"
#include "vapp/gui/i_fragment.hpp"
#include "vapp/core/event_system.hpp"
#include "vapp/core/resource_manager.hpp"
#include "vapp/core/timer.hpp"

namespace Vapp {

class Vapp {
   public:
    Vapp(AppParams appParams = AppParams());
    ~Vapp();
    void init();
    void initActions();

    std::shared_ptr<Actions> actions();
    std::shared_ptr<EventSystem> getEventSystem();
    std::shared_ptr<ResourceManager> getResourceManager();
    std::shared_ptr<Timer> timer();

    void attachFragment(std::unique_ptr<IFragment> fragment);

    void run();

   private:
    AppParams m_appParams;
    std::shared_ptr<Actions> m_actions;
    std::unique_ptr<Gui> m_gui;
    std::shared_ptr<EventSystem> m_eventSystem;
    std::shared_ptr<ResourceManager> m_resourceManager;
    std::shared_ptr<Timer> m_timer;
};

}  // namespace Vapp
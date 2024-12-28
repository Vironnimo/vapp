#pragma once

#include <memory>

#include "vapp/core/app_params.hpp"
#include "vapp/core/actions.hpp"
#include "vapp/gui/gui.hpp"
#include "vapp/gui/fragment/i_base_fragment.hpp"

namespace Vapp {

class Vapp {
   public:
    Vapp(AppParams appParams = AppParams());
    ~Vapp();
    void init();
    void initActions();

    std::shared_ptr<Actions> getActions();

    void attachFragment(std::unique_ptr<IBaseFragment> fragment);

    void run();

   private:
    AppParams m_appParams;
    std::shared_ptr<Actions> m_actions;
    std::unique_ptr<Gui> m_gui;
};

}  // namespace Vapp
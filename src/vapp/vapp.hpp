#pragma once

#include "vapp/core/app_params.hpp"

namespace Vapp {

class Vapp {
   public:
    Vapp(AppParams appParams = AppParams());
    ~Vapp();
    void init();

    void run();

   private:
    AppParams m_appParams;
};

}  // namespace Vapp
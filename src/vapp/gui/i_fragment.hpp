#pragma once
#include <memory>
#include <utility>

#include "vapp/gui/theme.hpp"
#include <imgui.h>

namespace Vapp {

class Vapp;

class IFragment {
   public:
    virtual ~IFragment() = default;
    virtual void draw() = 0;

    void setVapp(Vapp* vapp) { m_vapp = vapp; }
    void setStyle(std::shared_ptr<Theme> style) { m_style = std::move(style); }

   protected:
    std::shared_ptr<Theme> m_style;
    Vapp* m_vapp;
};

}  // namespace vUtility

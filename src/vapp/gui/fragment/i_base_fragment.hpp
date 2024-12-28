#pragma once
#include <memory>
#include <utility>

#include "vapp/gui/gui_style.hpp"
#include <imgui.h>

namespace Vapp {

class IBaseFragment {
   public:
    virtual ~IBaseFragment() = default;
    virtual void draw() = 0;

    void setIO(ImGuiIO* io) { m_io = io; }
    void setStyle(std::shared_ptr<GuiStyle> style) { m_style = std::move(style); }

   protected:
    ImGuiIO* m_io = nullptr;
    std::shared_ptr<GuiStyle> m_style;
};

}  // namespace vUtility

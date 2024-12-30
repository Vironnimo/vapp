#pragma once
#include <memory>
#include <utility>

#include "vapp/gui/theme.hpp"
#include "vapp/core/resource_manager.hpp"
#include <imgui.h>

namespace Vapp {

class IFragment {
   public:
    virtual ~IFragment() = default;
    virtual void draw() = 0;

    void setResourceManager(std::shared_ptr<ResourceManager> rm) { m_resourceManager = rm; }
    void setStyle(std::shared_ptr<Theme> style) { m_style = std::move(style); }

   protected:
    std::shared_ptr<Theme> m_style;
    std::shared_ptr<ResourceManager> m_resourceManager;
};

}  // namespace vUtility

#define MINIAUDIO_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "resource_manager.hpp"

#include <filesystem>

#include <spdlog/spdlog.h>

namespace Vapp {

ResourceManager::ResourceManager() {
    spdlog::debug("Constructor ResourceManager");
    init();
}

void ResourceManager::init() {
    m_resourceRoot = std::filesystem::path("resources");
}

ResourceManager::~ResourceManager() {
    spdlog::debug("Destructor ResourceManager");
    for (const auto& [id, resource] : m_resources) {
        auto* res = static_cast<IResource*>(resource.get());
        if (res) {
            res->unload();
        }
    }
}

}  // namespace Vapp
#include "resource_manager.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <spdlog/spdlog.h>


namespace Vapp {

ResourceManager::ResourceManager() {
    spdlog::debug("ResourceManager Constructor");
    init();
    // IN CMAKE COPY RESOURCES FOLDER AND MOVE FONTS THERE
}

void ResourceManager::init() {
    // sdl init
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        spdlog::warn("failed to init sdl: {}", SDL_GetError());
    }
    // sdl_mixer init
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        spdlog::warn("failed to init sdl mixer: {}", Mix_GetError());
        SDL_Quit();
    }
    // sdl_image init
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        spdlog::warn("failed to init sdl image: {}", IMG_GetError());
        Mix_CloseAudio();
        SDL_Quit();
    }
}

ResourceManager::~ResourceManager() {
    spdlog::debug("ResourceManager Destructor");
    for (auto [id, resource] : m_resources) {
        auto res = static_cast<IResource*>(resource.get());
        if (res) {
            res->unload();
        }
    }

    IMG_Quit();
    Mix_CloseAudio();
    SDL_Quit();
}

}  // namespace Vapp
#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#define SDL_MAIN_HANDLED  // sdl tries to inject its own main(), nope.
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <spdlog/spdlog.h>

namespace Vapp {

class IResource {
   public:
    virtual ~IResource() = default;
    virtual bool loadFromFile(const std::string& path) = 0;
    virtual void unload() = 0;

   protected:
    bool m_isLoaded = false;
};

class Sound : public IResource {
   public:
    bool loadFromFile(const std::string& path) override {
        m_chunk = Mix_LoadWAV(path.c_str());
        if (!m_chunk) {
            spdlog::info("failed to load sound: {}", Mix_GetError());
        }

        m_isLoaded = false;
        return true;
    }

    void unload() override {
        Mix_FreeChunk(m_chunk);
        m_chunk = nullptr;
        m_isLoaded = false;
    }

    void play() {
        spdlog::info("playing sound");
        Mix_PlayChannel(-1, m_chunk, 0);
    }

   private:
    Mix_Chunk* m_chunk = nullptr;
};

class Image : IResource {
   public:
    bool loadFromFile(const std::string& path) override {
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (!surface) {
            spdlog::info("failed to load image: {}", IMG_GetError());
            return false;
        }

        glGenTextures(1, &m_textureID);
        glBindTexture(GL_TEXTURE_2D, m_textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        m_width = surface->w;
        m_height = surface->h;
        m_isLoaded = true;

        SDL_FreeSurface(surface);

        return true;
    }

    void unload() override {
        if (m_isLoaded) {
            glDeleteTextures(1, &m_textureID);
            m_textureID = 0;
            m_isLoaded = false;
        }
    }

    ImTextureID getTexture() const {
        return (ImTextureID)(intptr_t)m_textureID;
    }

    GLuint getTextureID() const {
        return m_textureID;
    }

    int getWidth() const {
        return m_width;
    }

    int getHeight() const {
        return m_height;
    }

   private:
    GLuint m_textureID = 0;
    int m_width = 0;
    int m_height = 0;
};

class ResourceManager {
   public:
    ResourceManager();
    ~ResourceManager();
    void init();

    template <typename T>
    void load(const std::string& name, const std::string& path) {
        auto resource = std::make_shared<T>();
        if (resource->loadFromFile(path)) {
            m_resources[name] = resource;
        }
    }

    template <typename T>
    T* get(const std::string& name) {
        auto it = m_resources.find(name);
        if (it != m_resources.end()) {
            return static_cast<T*>(it->second.get());
        }

        return nullptr;
    }

   private:
    std::unordered_map<std::string, std::shared_ptr<void>> m_resources;
};

}  // namespace Vapp
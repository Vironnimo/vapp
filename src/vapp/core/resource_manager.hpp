#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

#define SDL_MAIN_HANDLED  // sdl tries to inject its own main(), nope.
#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <imgui.h>
#include <spdlog/spdlog.h>

namespace Vapp {

class IResource {
   public:
    virtual ~IResource() = default;
    virtual bool loadFromFile(const std::string& fileName) = 0;
    virtual bool unload() = 0;

   protected:
    bool m_isLoaded = false;
};

class Sound : public IResource {
   public:
    bool loadFromFile(const std::string& fileName) override {
        auto folder = std::filesystem::path("resources/sounds");
        auto fullPath = folder / fileName;
        m_chunk = Mix_LoadWAV(fullPath.string().c_str());
        if (!m_chunk) {
            spdlog::info("failed to load sound: {}", Mix_GetError());
            return false;
        }

        m_isLoaded = true;
        return true;
    }

    bool unload() override {
        if (!m_isLoaded) {
            return false;
        }
        Mix_FreeChunk(m_chunk);
        m_chunk = nullptr;
        m_isLoaded = false;

        return true;
    }

    void play() {
        Mix_PlayChannel(-1, m_chunk, 0);
    }

   private:
    Mix_Chunk* m_chunk = nullptr;
};

class Image : IResource {
   public:
    bool loadFromFile(const std::string& fileName) override {
        auto folder = std::filesystem::path("resources/images");
        auto fullPath = folder / fileName;
        SDL_Surface* surface = IMG_Load(fullPath.string().c_str());
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

    bool unload() override {
        if (!m_isLoaded) {
            return false;
        }
        glDeleteTextures(1, &m_textureID);
        m_textureID = 0;
        m_isLoaded = false;

        return true;
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
    // todo create method for adding a resource, but only load it once it's actually used (lazy load)
   public:
    ResourceManager();
    ~ResourceManager();
    void init();

    template <typename T>
    bool load(const std::string& name, const std::string& fileName) {
        auto resource = std::make_shared<T>();
        if (resource->loadFromFile(fileName)) {
            m_resources[name] = resource;
            return true;
        }

        return false;
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
    std::filesystem::path m_resourceRoot;

    std::unordered_map<std::string, std::shared_ptr<void>> m_resources;
};

}  // namespace Vapp
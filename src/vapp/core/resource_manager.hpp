#pragma once
// #define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <spdlog/spdlog.h>
#include <stb_image.h>

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
    Sound() : m_engine(nullptr) {
        // Initialize miniaudio engine for this sound
        ma_engine_config engineConfig = ma_engine_config_init();
        ma_result result = ma_engine_init(&engineConfig, &m_engine_instance);
        if (result == MA_SUCCESS) {
            m_engine = &m_engine_instance;
        }
    }

    ~Sound() override {
        unload();
        if (m_engine) {
            ma_engine_uninit(&m_engine_instance);
        }
    }

    bool loadFromFile(const std::string& fileName) override {
        if (!m_engine) {
            spdlog::error("miniaudio engine not initialized");
            return false;
        }

        auto folder = std::filesystem::path("resources/sounds");
        auto fullPath = folder / fileName;
        
        // Store the file path for playback
        m_filePath = fullPath.string();
        m_isLoaded = true;
        
        spdlog::info("Loaded sound: {}", fileName);
        return true;
    }

    bool unload() override {
        if (!m_isLoaded) {
            return false;
        }

        // Stop any currently playing looped sound
        if (m_loopedSound) {
            ma_sound_uninit(m_loopedSound.get());
            m_loopedSound.reset();
        }
        
        m_isLoaded = false;
        return true;
    }

    void play() {
        if (!m_engine || !m_isLoaded) {
            spdlog::warn("Cannot play sound - engine not initialized or sound not loaded");
            return;
        }

        // Play the sound using miniaudio engine
        ma_result result = ma_engine_play_sound(m_engine, m_filePath.c_str(), nullptr);
        if (result != MA_SUCCESS) {
            spdlog::error("Failed to play sound: {}", ma_result_description(result));
        }
    }

    void playLooped() {
        if (!m_engine || !m_isLoaded) {
            spdlog::warn("Cannot play sound - engine not initialized or sound not loaded");
            return;
        }

        // Stop any existing looped sound
        stopLooped();

        // Create a new sound with looping
        m_loopedSound = std::make_unique<ma_sound>();
        ma_result result = ma_sound_init_from_file(m_engine, m_filePath.c_str(), 0, nullptr, nullptr, m_loopedSound.get());
        if (result == MA_SUCCESS) {
            ma_sound_set_looping(m_loopedSound.get(), MA_TRUE);
            ma_sound_start(m_loopedSound.get());
        } else {
            spdlog::error("Failed to create looped sound: {}", ma_result_description(result));
            m_loopedSound.reset();
        }
    }

    void stopLooped() {
        if (m_loopedSound) {
            ma_sound_stop(m_loopedSound.get());
            ma_sound_uninit(m_loopedSound.get());
            m_loopedSound.reset();
        }
    }

    void setVolume(float volume) {
        m_volume = std::clamp(volume, 0.0f, 1.0f);
        
        // Apply volume to looped sound if it exists
        if (m_loopedSound) {
            ma_sound_set_volume(m_loopedSound.get(), m_volume);
        }
    }

    float getVolume() const {
        return m_volume;
    }

   private:
    ma_engine* m_engine;
    ma_engine m_engine_instance;
    std::string m_filePath;
    std::unique_ptr<ma_sound> m_loopedSound;
    float m_volume = 1.0f;
};

class Image : public IResource {
   public:
    bool loadFromFile(const std::string& fileName) override {
        auto folder = std::filesystem::path("resources/images");
        auto fullPath = folder / fileName;
        
        int width, height, channels;
        unsigned char* data = stbi_load(fullPath.string().c_str(), &width, &height, &channels, 4); // Force RGBA
        
        if (!data) {
            spdlog::error("Failed to load image: {}", stbi_failure_reason());
            return false;
        }

        glGenTextures(1, &m_textureID);
        glBindTexture(GL_TEXTURE_2D, m_textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        m_width = width;
        m_height = height;
        m_isLoaded = true;

        stbi_image_free(data);
        spdlog::info("Loaded image: {} ({}x{})", fileName, width, height);
        
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
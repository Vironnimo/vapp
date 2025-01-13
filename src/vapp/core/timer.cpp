#include "timer.hpp"

#include <chrono>
#include <thread>
#include <spdlog/spdlog.h>

namespace Vapp {

Timer::Timer() : m_lastFrameTime(std::chrono::high_resolution_clock::now()) {
    spdlog::debug("Constructor Timer");
}

Timer::~Timer() {
    spdlog::debug("Destructor Timer");
}

int64_t Timer::time() {
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(now);
    return time.count();
}

void Timer::start(const std::string& timerId) {
    m_timers.emplace(timerId, std::chrono::high_resolution_clock::now());
}

int64_t Timer::get(const std::string& timerId) {
    auto it = m_timers.find(timerId);
    if (it == m_timers.end()) {
        return 0;
    }

    auto now = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(now - it->second);

    return time.count();
}

int64_t Timer::end(const std::string& timerId) {
    auto it = m_timers.find(timerId);
    if (it == m_timers.end()) {
        return 0;
    }

    auto now = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(now - it->second);
    m_timers.erase(it);

    return time.count();
}

void Timer::update() {
    // limit fps, here for now
    if (m_maxFps > 0) {
        const float targetFrameTime = 1.0f / static_cast<float>(m_maxFps);
        auto frameEnd = m_lastFrameTime + std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(
                                              std::chrono::duration<float>(targetFrameTime));

        if (std::chrono::high_resolution_clock::now() < frameEnd) {
            std::this_thread::sleep_until(frameEnd);
        }
    }

    auto now = std::chrono::high_resolution_clock::now();
    m_deltaTime = std::chrono::duration<float>(now - m_lastFrameTime).count();
    m_lastFrameTime = now;

    // fps
    m_frameCount++;
    m_fpsTimer += m_deltaTime;

    if (m_fpsTimer >= 1.0f) {
        m_fps = static_cast<float>(m_frameCount) / m_fpsTimer;
        m_frameCount = 0;
        m_fpsTimer = 0.0f;
    }
}

float Timer::deltaTime() {
    return m_deltaTime;
}

float Timer::getFps() {
    return m_fps;
}

void Timer::setMaxFps(int fps) {
    m_maxFps = fps;
}

}  // namespace Vapp
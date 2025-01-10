#include "timer.hpp"

#include <chrono>
#include <thread>

namespace Vapp {

Timer::Timer() : m_lastFrameTime(std::chrono::high_resolution_clock::now()) {
}

void Timer::update() {
    // limit fps, here for now
    if (m_maxFps > 0) {
        const float targetFrameTime = 1.0f / static_cast<float>(m_maxFps);
        auto frameEnd = m_lastFrameTime + std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(
                                              std::chrono::duration<float>(targetFrameTime));

        while (std::chrono::high_resolution_clock::now() < frameEnd) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    auto currentTime = std::chrono::high_resolution_clock::now();
    m_deltaTime = std::chrono::duration<float>(currentTime - m_lastFrameTime).count();
    m_lastFrameTime = currentTime;

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
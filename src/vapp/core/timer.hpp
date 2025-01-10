#pragma once

#include <chrono>

namespace Vapp {

// todo use global (inside namespace) variable for this

class Timer {
    public:
     Timer();
     void update();
     float deltaTime();
     float getFps();
     void setMaxFps(int fps);

    private:
     float m_deltaTime = 0.0f;
     float m_fps = 0.0f;
     float m_fpsTimer = 0.0f;
     int m_frameCount = 0;
     int m_maxFps = 0;
     std::chrono::high_resolution_clock::time_point m_lastFrameTime;
};

}  // namespace Vapp
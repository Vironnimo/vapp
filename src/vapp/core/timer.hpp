#pragma once

#include <chrono>
#include <map>
#include <string>

namespace Vapp {

// todo use global (inside namespace) variable for this

class Timer {
   public:
    Timer();
    ~Timer();

    // get current time in milliseconds
    int64_t time();
    // start a new timer
    void start(const std::string& timerId);
    // get elapsed time since start of the timer in milliseconds, without ending it
    int64_t get(const std::string& timerId);
    // end timer and get time in milliseconds
    int64_t end(const std::string& timerId);

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
    std::map<std::string, std::chrono::high_resolution_clock::time_point> m_timers;
};

}  // namespace Vapp
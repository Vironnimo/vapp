#pragma once
#include <string>

namespace Vapp {

enum LogLevel { debug, info, warn, error };

class Logger {
   public:
    Logger();
    ~Logger();
    void useFileLogger(const std::string& logFile);
    void setLogLevel(LogLevel level);

    template <typename... Args>
    void debug(const std::string& fmt, const Args&... args) {
        spdlog::debug(fmt, args...);
    }

    template <typename... Args>
    void info(const std::string& fmt, const Args&... args) {
        spdlog::info(fmt, args...);
    }

    template <typename... Args>
    void warn(const std::string& fmt, const Args&... args) {
        spdlog::warn(fmt, args...);
    }

    template <typename... Args>
    void error(const std::string& fmt, const Args&... args) {
        spdlog::error(fmt, args...);
    }

   private:
};

extern Logger log;

}  // namespace Vapp
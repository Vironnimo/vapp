#include "logger.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

namespace Vapp {

Logger log;

Logger::Logger() {
    spdlog::set_level(spdlog::level::debug);
    spdlog::flush_on(spdlog::level::debug);
    spdlog::debug("Constructor Logger");
}

Logger::~Logger() {
    spdlog::debug("Destructor Logger");
}

void Logger::useFileLogger(const std::string& logFile) {
    auto fileLogger = spdlog::basic_logger_mt("file_logger", logFile);
    spdlog::set_default_logger(fileLogger);
}

void Logger::setLogLevel(LogLevel level) {
    switch (level) {
        case LogLevel::debug:
    spdlog::set_level(spdlog::level::debug);
            break;
        case LogLevel::info:
    spdlog::set_level(spdlog::level::info);
            break;
        case LogLevel::warn:
    spdlog::set_level(spdlog::level::warn);
            break;
        case LogLevel::error:
    spdlog::set_level(spdlog::level::err);
            break;
    }
}

}  // namespace Vapp
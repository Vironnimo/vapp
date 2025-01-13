#pragma once

#include <string>
#include <functional>
#include "vapp/core/logger.hpp"

namespace Vapp {

struct AppParams {
    // window settings
    std::string windowTitle = "My App";
    bool windowCentered = true;
    int windowWidth = 640;
    int windowHeight = 480;
    int windowWidthMin = 0;
    int windowHeightMin = 0;
    int maxFps = 0;

    // todo should be just a method in gui like setMenuCallback?
    std::function<void()> menuCallback;

    // logger settings
    LogLevel logLevel = LogLevel::debug;
    bool useFileLogger = false;
    std::string loggerFile = "vapp.log";
};

}  // namespace Vapp
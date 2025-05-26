#pragma once

#include <string>
#include <functional>

namespace Vapp {

struct AppSettings {
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
};

}  // namespace Vapp
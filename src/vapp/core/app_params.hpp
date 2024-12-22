#pragma once

#include <string>
#include <functional>

namespace Vapp {

struct AppParams {
    std::string window_title = "My App";
    bool window_centered = true;
    int window_width = 800;
    int window_height = 800;
    int window_min_width = 800;
    int window_min_height = 800;

    std::function<void()> menuCallback;
};

}  // namespace Vapp
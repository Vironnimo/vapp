#include "vapp.hpp"

#include <spdlog/common.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <utility>
#include <vector>

#include "gui/gui.hpp"
#include "vapp/core/app_params.hpp"

namespace Vapp {

Vapp::Vapp(AppParams appParams) : m_appParams(std::move(appParams)) {
    init();
    spdlog::debug("Vapp Constructor");
}

void Vapp::init() {
#if defined(_DEBUG)
    spdlog::set_level(spdlog::level::debug);
    spdlog::debug("Build Configuration: Debug");
#else
    // log to file
    auto file_logger = spdlog::basic_logger_mt("file_logger", "logs.txt");
    spdlog::set_default_logger(file_logger);
#endif
}

void Vapp::run() {
    spdlog::debug("run App");
    Gui gui(m_appParams);

    while (!gui.windowShouldClose) {
        gui.render();
    }
}

Vapp::~Vapp() {
    spdlog::debug("Vapp Destructor");
}

}  // namespace Vapp
#include "example_class.hpp"

#include <spdlog/spdlog.h>

#include "vapp/core/actions.hpp"

ExampleClass::ExampleClass(std::shared_ptr<Vapp::Actions> actions) : m_actions(std::move(actions)) {
    m_actions->registerAction("app.example", "Example Action", [this]() {
        spdlog::debug("yeah, here is an example with the member variable value: {}", m_testValue);
        m_testValue++;
    });
    m_actions->addKeybinding("app.example", {GLFW_KEY_S, false, false, false});
}

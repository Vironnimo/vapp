#include "example_class.hpp"

#include <spdlog/spdlog.h>

#include "vapp/vapp.hpp"

ExampleClass::ExampleClass(std::shared_ptr<Vapp::Vapp> vapp) : m_vapp(std::move(vapp)) {
    m_vapp->getActions()->registerAction("app.example", "Example Action", [this]() {
        spdlog::debug("yeah, here is an example with the member variable value: {}", m_testValue);
        m_testValue++;
    });
    m_vapp->getActions()->addKeybinding("app.example", {GLFW_KEY_S, false, false, false});

    m_vapp->getEventSystem()->emit("example_event");

    m_vapp->getActions()->registerAction("app.event_test", "Example Action with Event trigger", [this]() { 
        m_vapp->getEventSystem()->emit("example_event", &m_testValue); 
    });
    m_vapp->getActions()->addKeybinding("app.event_test", {GLFW_KEY_F, false, false, false});
}

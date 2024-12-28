#include "actions.hpp"

#include <memory>

namespace Vapp {

void Actions::execute(const std::string& id) {
    auto it = m_actions.find(id);
    if (it != m_actions.end()) {
        it->second->execute();
    }
}

void Actions::registerAction(const std::string& id, const std::string& name, std::function<void()> callback) {
    m_actions.emplace(id, std::make_unique<Action>(id, name, std::move(callback)));
}

void Actions::addKeybinding(const std::string& actionId, KeyBinding keyBind) {
    auto it = m_actions.find(actionId);
    if (it != m_actions.end()) {
        it->second->addKeyBinding(keyBind);
    }
}

bool Actions::isKeyBindingPressed(GLFWwindow* window, const KeyBinding& keyBind) {
    const bool keyPressed = glfwGetKey(window, keyBind.key) == GLFW_PRESS;
    const bool ctrlHeld = keyBind.ctrl == (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
                                           glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS);
    const bool shiftHeld =
        keyBind.shift == (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);
    const bool altHeld =
        keyBind.alt == (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS);

    return keyPressed && ctrlHeld && shiftHeld && altHeld;
}

void Actions::handleInput(GLFWwindow* window) {
    for (const auto& [id, action] : m_actions) {
        for (const auto& binding : action->getKeyBindings()) {
            if (isKeyBindingPressed(window, binding)) {
                action->execute();
                break;
            }
        }
    }
}

}  // namespace Vapp
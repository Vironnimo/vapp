#pragma once

#include <functional>
#include <memory>
#include <string>
#include <GLFW/glfw3.h>

namespace Vapp {

struct KeyBinding {
    int key;  // GLFW key id
    bool shift = false;
    bool ctrl = false;
    bool alt = false;
};

class Action {
   public:
    Action(std::string id, std::string name, std::function<void()> callback)
        : m_id(std::move(id)), m_name(std::move(name)), m_callback(std::move(callback)) {
    }

    void execute() {
        if (m_callback) {
            m_callback();
        }
    }

    void addKeyBinding(KeyBinding keyBind) {
        m_keyBindings.push_back(keyBind);
    }

    const std::vector<KeyBinding>& getKeyBindings() {
        return m_keyBindings;
    }

    const std::string& getId() const {
        return m_id;
    }
    const std::string& getName() const {
        return m_name;
    }

   private:
    std::string m_id;
    std::string m_name;
    std::function<void()> m_callback;
    std::vector<KeyBinding> m_keyBindings;
};

// todo add cooldown to hotkeys so we don't have rapid fire

class Actions {
   public:
    Actions();
    ~Actions();

    void execute(const std::string& id);
    void add(const std::string& id, const std::string& name, std::function<void()> callback);
    void addKeybinding(const std::string& actionId, KeyBinding keyBind);

    bool isKeyBindingPressed(GLFWwindow* window, const KeyBinding& keyBind);
    void handleInput(GLFWwindow* window);

   private:
    std::unordered_map<std::string, std::unique_ptr<Action>> m_actions;
};

}  // namespace Vapp
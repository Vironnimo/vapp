#include <GLFW/glfw3.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include "vapp/core/actions.hpp"


class ActionsTest : public ::testing::Test {
   protected:
    Vapp::Actions actions;
};

TEST_F(ActionsTest, AddAction) {
    actions.add("test_action", "Test Action", []() { spdlog::info("Test Action Executed"); });
    EXPECT_NO_THROW(actions.execute("test_action"));
}

TEST_F(ActionsTest, ExecuteAction) {
    bool executed = false;
    actions.add("test_action", "Test Action", [&executed]() { executed = true; });
    actions.execute("test_action");
    EXPECT_TRUE(executed);
}

TEST_F(ActionsTest, ExecuteNonExistentAction) {
    EXPECT_NO_THROW(actions.execute("non_existent_action"));
}

TEST_F(ActionsTest, AddDuplicateAction) {
    actions.add("test_action", "Test Action", []() { spdlog::info("Test Action Executed"); });
    EXPECT_FALSE(actions.add("test_action", "Test Action Duplicate", []() { spdlog::info("Test Action Duplicate Executed"); }));
}

TEST_F(ActionsTest, ExecuteActionWithoutCallback) {
    actions.add("test_action", "Test Action", nullptr);
    EXPECT_NO_THROW(actions.execute("test_action"));
}

TEST_F(ActionsTest, AddKeybindingToNonExistentAction) {
    Vapp::KeyBinding keyBind{GLFW_KEY_A, true, false, false};
    EXPECT_NO_THROW(actions.addKeybinding("non_existent_action", keyBind));
}

// TEST_F(ActionsTest, AddKeybinding) {
//     Vapp::KeyBinding keyBind{GLFW_KEY_A, true, false, false};
//     actions.add("test_action", "Test Action", []() { spdlog::info("Test Action Executed"); });
//     actions.addKeybinding("test_action", keyBind);
    // Assuming getKeyBindings is public or accessible for testing
    // auto action = actions.m_actions.find("test_action");
    // ASSERT_NE(action, actions.m_actions.end());
    // EXPECT_EQ(action->second->getKeyBindings().size(), 1);
// }

// how do we actually simulate a keystroke?
// TEST_F(ActionsTest, HandleInput) {
//     if (!glfwInit()) {
//         FAIL() << "Failed to initialize GLFW";
//     }

//     GLFWwindow* window = glfwCreateWindow(640, 480, "Test Window", nullptr, nullptr);
//     ASSERT_NE(window, nullptr);

//     bool executed = false;
//     Vapp::KeyBinding keyBind{GLFW_KEY_A, true, false, false};
//     actions.add("test_action", "Test Action", [&executed]() { executed = true; });
//     actions.addKeybinding("test_action", keyBind);

//     // Set the key callback
//     glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
//         if (key == GLFW_KEY_A && action == GLFW_PRESS) {
//             spdlog::info("Key A pressed");
//         }
//         if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) {
//             spdlog::info("Left Shift pressed");
//         }
//     });

//     // Simulate key press
//     glfwSetWindowUserPointer(window, &actions);
//     glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
//         auto actions = static_cast<Vapp::Actions*>(glfwGetWindowUserPointer(window));
//         if (key == GLFW_KEY_A && action == GLFW_PRESS) {
//             actions->execute("test_action");
//         }
//     });

//     // Simulate key press event
//     glfwPostEmptyEvent();
//     glfwPollEvents();
//     actions.handleInput(window);

//     EXPECT_TRUE(executed);

//     glfwDestroyWindow(window);
//     glfwTerminate();
// }
#include <gtest/gtest.h>
#include "vapp/core/event_system.hpp"

class EventSystemTest : public ::testing::Test {
   protected:
    Vapp::EventSystem eventSystem;
};

TEST_F(EventSystemTest, SubscribeAndEmit) {
    bool eventTriggered = false;
    eventSystem.subscribe("test_event", [&eventTriggered](void* data) {
        eventTriggered = true;
    });

    eventSystem.emit("test_event");
    EXPECT_TRUE(eventTriggered);
}

TEST_F(EventSystemTest, EmitWithData) {
    int eventData = 0;
    eventSystem.subscribe("test_event", [&eventData](void* data) {
        eventData = *static_cast<int*>(data);
    });

    int data = 42;
    eventSystem.emit("test_event", &data);
    EXPECT_EQ(eventData, 42);
}

TEST_F(EventSystemTest, MultipleSubscribers) {
    int eventData1 = 0;
    int eventData2 = 0;

    eventSystem.subscribe("test_event", [&eventData1](void* data) {
        eventData1 = *static_cast<int*>(data);
    });

    eventSystem.subscribe("test_event", [&eventData2](void* data) {
        eventData2 = *static_cast<int*>(data);
    });

    int data = 42;
    eventSystem.emit("test_event", &data);
    EXPECT_EQ(eventData1, 42);
    EXPECT_EQ(eventData2, 42);
}

TEST_F(EventSystemTest, NoSubscribers) {
    // emit event with no subscribers, should not cause any issues
    EXPECT_NO_THROW(eventSystem.emit("non_existent_event"));
}

TEST_F(EventSystemTest, EmitWithoutSubscription) {
    bool eventTriggered = false;
    eventSystem.emit("test_event");
    EXPECT_FALSE(eventTriggered);  
}

TEST_F(EventSystemTest, EmitWithIncorrectData) {
    int eventData = 0;
    eventSystem.subscribe("test_event", [&eventData](void* data) {
        eventData = *static_cast<int*>(data);
    });

    float incorrectData = 42.0f;
    eventSystem.emit("test_event", &incorrectData);
    EXPECT_NE(eventData, 42);  // incorrect type, shoudn't change
}

TEST_F(EventSystemTest, EmitWithNullData) {
    int eventData = 0;
    eventSystem.subscribe("test_event", [&eventData](void* data) {
        if (data) {
            eventData = *static_cast<int*>(data);
        }
    });

    eventSystem.emit("test_event", nullptr);
    EXPECT_EQ(eventData, 0); // no data, no change
}

TEST_F(EventSystemTest, MultipleSubscribersWithDifferentData) {
    int eventData1 = 0;
    float eventData2 = 0.0f;

    eventSystem.subscribe("test_event", [&eventData1](void* data) {
        eventData1 = *static_cast<int*>(data);
    });

    eventSystem.subscribe("test_event", [&eventData2](void* data) {
        eventData2 = *static_cast<float*>(data);
    });

    int data = 42;
    eventSystem.emit("test_event", &data);
    EXPECT_EQ(eventData1, 42);  // should change
    EXPECT_NE(eventData2, 42.0f);  // shoudn't change because wrong datatype
}
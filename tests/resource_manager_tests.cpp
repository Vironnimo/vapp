#include <gtest/gtest.h>

#include "vapp/core/resource_manager.hpp"

class ResourceManagerTest : public ::testing::Test {
   protected:
    Vapp::ResourceManager resourceManager;
};

TEST_F(ResourceManagerTest, Constructor) {
    EXPECT_NO_THROW(Vapp::ResourceManager());
}

TEST_F(ResourceManagerTest, LoadSound) {
    EXPECT_TRUE(resourceManager.load<Vapp::Sound>("test_sound", "test_sound.wav"));
    auto sound = resourceManager.get<Vapp::Sound>("test_sound");
    ASSERT_NE(sound, nullptr);
}

TEST_F(ResourceManagerTest, LoadImage) {
    EXPECT_TRUE(resourceManager.load<Vapp::Image>("test_image", "test_image.png"));
    auto image = resourceManager.get<Vapp::Image>("test_image");
    ASSERT_NE(image, nullptr);
}

TEST_F(ResourceManagerTest, GetNonExistentResource) {
    auto sound = resourceManager.get<Vapp::Sound>("non_existent_sound");
    EXPECT_EQ(sound, nullptr);
}

TEST_F(ResourceManagerTest, LoadInvalidSoundFile) {
    EXPECT_FALSE(resourceManager.load<Vapp::Sound>("invalid_sound", "invalid.wav"));
    auto sound = resourceManager.get<Vapp::Sound>("invalid_sound");
    ASSERT_EQ(sound, nullptr);
}

TEST_F(ResourceManagerTest, LoadInvalidImageFile) {
    EXPECT_FALSE(resourceManager.load<Vapp::Image>("invalid_image", "invalid.png"));
    auto image = resourceManager.get<Vapp::Image>("invalid_image");
    ASSERT_EQ(image, nullptr);
}

TEST_F(ResourceManagerTest, UnloadSound) {
    resourceManager.load<Vapp::Sound>("test_sound", "test_sound.wav");
    auto sound = resourceManager.get<Vapp::Sound>("test_sound");
    ASSERT_NE(sound, nullptr);
    EXPECT_TRUE(sound->unload());
}

TEST_F(ResourceManagerTest, UnloadImage) {
    resourceManager.load<Vapp::Image>("test_image", "test_image.png");
    auto image = resourceManager.get<Vapp::Image>("test_image");
    ASSERT_NE(image, nullptr);
    EXPECT_TRUE(image->unload());
}
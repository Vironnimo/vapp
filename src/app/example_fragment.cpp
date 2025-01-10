#include "example_fragment.hpp"

#include <imgui.h>
#include <spdlog/spdlog.h>


TestFragment::TestFragment() {
    spdlog::debug("TestFragment Constructor");
}

TestFragment::~TestFragment() {
    spdlog::debug("TestFragment Destructor");
}

void TestFragment::draw() {
    ImGui::BeginChild("example");

    ImGui::Text("geiles zeug hier....");

    if (ImGui::Button("Click Sound")) {
        auto sound = m_resourceManager->get<Vapp::Sound>("click");
        sound->play();
    }

    auto img = m_resourceManager->get<Vapp::Image>("snake");
    ImGui::Image(img->getTexture(), ImVec2(img->getWidth() * 0.5, img->getHeight() * 0.5));

    ImGui::EndChild();
}

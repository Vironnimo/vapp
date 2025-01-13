#include "example_fragment.hpp"

#include <imgui.h>
#include <spdlog/spdlog.h>

TestFragment::TestFragment() {
    spdlog::debug("Constructor TestFragment");
}

TestFragment::~TestFragment() {
    spdlog::debug("Destructor TestFragment");
}

void TestFragment::draw() {
    ImGui::BeginChild("example");

    ImGui::Text("geiles zeug hier....");
    // font awesome usage
    m_style->pushFont(Vapp::Awesome);
    ImGui::Text(ICON_FA_ADDRESS_BOOK " " ICON_FA_ADDRESS_CARD);
    ImGui::Text(ICON_FA_HDD);
    m_style->popFont();

    if (ImGui::Button("Click Sound")) {
        spdlog::info("Button: playing click sound");
        auto sound = m_resourceManager->get<Vapp::Sound>("click");
        sound->play();
    }

    auto img = m_resourceManager->get<Vapp::Image>("snake");
    ImGui::Image(img->getTexture(), ImVec2(img->getWidth() * 0.5, img->getHeight() * 0.5));

    ImGui::EndChild();
}

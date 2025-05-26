#include "example_fragment.hpp"

#include "vapp/vapp.hpp"

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
        auto* sound = m_vapp->resources()->get<Vapp::Sound>("click");
        sound->play();
    }

    if (ImGui::Button("Start Recording")) {
        spdlog::info("Button: starting recording");
        m_vapp->audioRecorder()->startRecord();
    }

    if (ImGui::Button("Stop Recording")) {
        spdlog::info("Button: stopping recording");
        m_vapp->audioRecorder()->stopRecord();
        m_vapp->audioRecorder()->saveToFile("resources/sounds/recording_from_inside.wav");
    }

    if (ImGui::Button("Speech to Text")) {
        spdlog::info("Button: converting speech to text");
        auto result = m_vapp->speechToText()->read("resources/sounds/recording_from_inside.wav");
        spdlog::info("Transcription: {}", result);

        auto response = m_vapp->aiRequest()->requestCommand(result);
        spdlog::info("AI Command: {}", response.command);
    }

    auto* img = m_vapp->resources()->get<Vapp::Image>("snake");
    ImGui::Image(img->getTexture(), ImVec2(img->getWidth() * 0.5, img->getHeight() * 0.5));

    ImGui::EndChild();
}

#include "speech_to_text.hpp"
#include <fstream>
#include <vector>
#include <vosk/vosk_api.h>

namespace Vapp {

SpeechToText::SpeechToText() {
}

SpeechToText::~SpeechToText() {
    if (m_model) {
        vosk_model_free(m_model);
    }
}

void SpeechToText::init() {
    // vosk stuff
    // m_model = vosk_model_new("resources/vosk/small-en-us-0.15");
    // m_model = vosk_model_new("resources/vosk/small-de-0.15");
    m_model = vosk_model_new("resources/vosk/de-0.21");
}

std::string SpeechToText::read(const std::string &filePath) {
    VoskRecognizer *rec = vosk_recognizer_new(m_model, 16000.0f);
    std::ifstream wf(filePath, std::ios::binary);

    wf.seekg(44);  // skip WAV header
    std::vector<char> buf(4096);

    while (wf.read(buf.data(), buf.size()) || wf.gcount()) {
        vosk_recognizer_accept_waveform(rec, buf.data(), wf.gcount());
    }

    const auto* result = vosk_recognizer_final_result(rec);
    vosk_recognizer_free(rec);

    return result;
}

}  // namespace Vapp
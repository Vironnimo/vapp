#pragma once

#include <string>
#include <vosk/vosk_api.h>

namespace Vapp {
class SpeechToText {
   public:
    SpeechToText();
    ~SpeechToText();

    void init();

    std::string read(const std::string& filePath);

   private:
    VoskModel* m_model = nullptr;  
};

}  // namespace Vapp
#pragma once

#include "miniaudio.h"
#include <spdlog/spdlog.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>

namespace Vapp {

class AudioRecorder {
   public:
    AudioRecorder();
    ~AudioRecorder();

    bool initialize();
    bool startRecord();
    bool stopRecord();
    bool saveToFile(const std::string& filePath);
    bool isRecording() const;
    size_t getSampleCount() const;
    void setAmplification(float amplification);
    float getAmplification() const;

   private:
    // callback for miniaudio
    static void dataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
    void writeWavHeader(std::ofstream& file, int channels, int sampleRate, int bitDepth);
    void updateWavHeader(std::ofstream& file, size_t dataSize);

    bool m_initialized = false;
    std::atomic<bool> m_recording = false;
    ma_device m_device;
    std::vector<int16_t> m_recordedSamples;
    mutable std::mutex m_samplesMutex;
    float m_amplification = 1.0f;
};

}  // namespace Vapp
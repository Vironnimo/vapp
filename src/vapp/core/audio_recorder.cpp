#include "audio_recorder.hpp"

namespace Vapp {

AudioRecorder::AudioRecorder() {
}

AudioRecorder::~AudioRecorder() {
    if (m_recording) {
        stopRecord();
    }

    if (m_initialized) {
        ma_device_uninit(&m_device);
    }
}

bool AudioRecorder::initialize() {
    ma_device_config config = ma_device_config_init(ma_device_type_capture);
    config.capture.format = ma_format_s16;  // 16-bit signed integer
    config.capture.channels = 1;            // mono
    config.sampleRate = 16000;              // vosk expects 16000 Hz
    config.dataCallback = dataCallback;
    config.pUserData = this;

    ma_result result = ma_device_init(NULL, &config, &m_device);
    if (result != MA_SUCCESS) {
        spdlog::error("miniaudio initialization failed: {}", ma_result_description(result));
        return false;
    }

    m_initialized = true;
    spdlog::info("miniaudio initialized successfully");
    return true;
}

bool AudioRecorder::startRecord() {
    if (!m_initialized) {
        spdlog::error("miniaudio not initialized");
        return false;
    }
    if (m_recording.load()) {
        spdlog::warn("Already recording");
        return false;
    }

    // clear old stuff
    {
        std::lock_guard<std::mutex> lock(m_samplesMutex);
        m_recordedSamples.clear();
    }

    m_recording = true;

    ma_result result = ma_device_start(&m_device);
    if (result != MA_SUCCESS) {
        spdlog::error("Failed to start audio device: {}", ma_result_description(result));
        m_recording = false;
        return false;
    }

    spdlog::info("Recording started...");
    return true;
}

bool AudioRecorder::stopRecord() {
    if (!m_recording.load()) {
        spdlog::warn("Not currently recording");
        return false;
    }

    m_recording = false;
    ma_device_stop(&m_device);

    size_t sampleCount;
    {
        std::lock_guard<std::mutex> lock(m_samplesMutex);
        sampleCount = m_recordedSamples.size();
    }

    spdlog::info("Recording stopped. Captured {} samples", sampleCount);
    return true;
}

bool AudioRecorder::saveToFile(const std::string& filePath) {
    std::lock_guard<std::mutex> lock(m_samplesMutex);

    if (m_recordedSamples.empty()) {
        spdlog::error("No audio data to save");
        return false;
    }

    const int sampleRate = 16000;
    const int channels = 1;

    std::ofstream outFile(filePath, std::ios::binary);
    if (!outFile) {
        spdlog::error("Failed to open output file: {}", filePath);
        return false;
    }

    // WAV header
    writeWavHeader(outFile, channels, sampleRate, 16);
    // audio data
    outFile.write(reinterpret_cast<const char*>(m_recordedSamples.data()), m_recordedSamples.size() * sizeof(int16_t));
    // update WAV header with correct size
    updateWavHeader(outFile, m_recordedSamples.size() * sizeof(int16_t));

    spdlog::info("Recording saved to {}", filePath);
    return true;
}

bool AudioRecorder::isRecording() const {
    return m_recording.load();
}

size_t AudioRecorder::getSampleCount() const {
    std::lock_guard<std::mutex> lock(m_samplesMutex);
    return m_recordedSamples.size();
}

void AudioRecorder::setAmplification(float amplification) {
    std::lock_guard<std::mutex> lock(m_samplesMutex);
    m_amplification = std::clamp(amplification, 0.1f, 10.0f);  // Allow 0.1x to 10x amplification
    spdlog::info("Audio amplification set to {}x", m_amplification);
}

float AudioRecorder::getAmplification() const {
    std::lock_guard<std::mutex> lock(m_samplesMutex);
    return m_amplification;
}

void AudioRecorder::dataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    AudioRecorder* recorder = static_cast<AudioRecorder*>(pDevice->pUserData);

    if (!recorder->m_recording.load()) {
        return;
    }

    const int16_t* samples = static_cast<const int16_t*>(pInput);
    {
        std::lock_guard<std::mutex> lock(recorder->m_samplesMutex);

        // amplify (make it louder)
        for (ma_uint32 i = 0; i < frameCount; ++i) {
            float amplified = static_cast<float>(samples[i]) * recorder->m_amplification;
            amplified = std::clamp(amplified, -32767.0f, 32767.0f);
            recorder->m_recordedSamples.push_back(static_cast<int16_t>(amplified));
        }
    }
}

void AudioRecorder::writeWavHeader(std::ofstream& file, int channels, int sampleRate, int bitDepth) {
    // standard WAV header
    // RIFF chunk
    file.write("RIFF", 4);
    file.write("\0\0\0\0", 4);
    file.write("WAVE", 4);

    // fmt chunk
    file.write("fmt ", 4);
    int32_t fmtSize = 16;
    file.write(reinterpret_cast<const char*>(&fmtSize), 4);
    int16_t audioFormat = 1;
    file.write(reinterpret_cast<const char*>(&audioFormat), 2);
    int16_t numChannels = static_cast<int16_t>(channels);
    file.write(reinterpret_cast<const char*>(&numChannels), 2);
    int32_t sampleRateVal = sampleRate;
    file.write(reinterpret_cast<const char*>(&sampleRateVal), 4);
    int32_t byteRate = sampleRate * channels * (bitDepth / 8);
    file.write(reinterpret_cast<const char*>(&byteRate), 4);
    int16_t blockAlign = channels * (bitDepth / 8);
    file.write(reinterpret_cast<const char*>(&blockAlign), 2);
    int16_t bitsPerSample = static_cast<int16_t>(bitDepth);
    file.write(reinterpret_cast<const char*>(&bitsPerSample), 2);

    // data chunk
    file.write("data", 4);
    file.write("\0\0\0\0", 4);
}

void AudioRecorder::updateWavHeader(std::ofstream& file, size_t dataSize) {
    file.seekp(4);
    int32_t riffSize = static_cast<int32_t>(dataSize + 36);
    file.write(reinterpret_cast<const char*>(&riffSize), 4);

    file.seekp(40);
    int32_t dataChunkSize = static_cast<int32_t>(dataSize);
    file.write(reinterpret_cast<const char*>(&dataChunkSize), 4);
}

}  // namespace Vapp
#include <portaudio.h>
#include <spdlog/spdlog.h>
#include <fstream>
#include <vector>

class AudioRecorder {
   public:
    AudioRecorder() : m_initialized(false), m_recording(false), m_stream(nullptr) {
    }

    ~AudioRecorder() {
        if (m_initialized) {
            Pa_Terminate();
        }
    }

    bool initialize() {
        PaError err = Pa_Initialize();
        if (err != paNoError) {
            spdlog::error("PortAudio initialization failed: {}", Pa_GetErrorText(err));
            return false;
        }
        m_initialized = true;
        return true;
    }

    bool recordToFile(const std::string& filePath, int durationSec = 5) {
        if (!m_initialized) {
            spdlog::error("PortAudio not initialized");
            return false;
        }

        // Parameters for recording
        const int sampleRate = 16000;  // Same as Vosk expects
        const int framesPerBuffer = 1024;
        const int channels = 1;  // Mono

        // Open output WAV file
        std::ofstream outFile(filePath, std::ios::binary);
        if (!outFile) {
            spdlog::error("Failed to open output file: {}", filePath);
            return false;
        }

        // Write WAV header (will be updated later with correct size)
        writeWavHeader(outFile, channels, sampleRate, 16);  // 16-bit PCM

        // Prepare buffer for recording
        std::vector<int16_t> buffer(framesPerBuffer * channels);
        m_recordedSamples.clear();
        m_recording = true;

        // Open audio stream
        PaError err;
        PaStreamParameters inputParams;
        inputParams.device = Pa_GetDefaultInputDevice();
        inputParams.channelCount = channels;
        inputParams.sampleFormat = paInt16;
        inputParams.suggestedLatency = Pa_GetDeviceInfo(inputParams.device)->defaultLowInputLatency;
        inputParams.hostApiSpecificStreamInfo = nullptr;

        // Define callback function
        auto callback = [](const void* inputBuffer,
                           void* outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void* userData) -> int {
            auto* recorder = static_cast<AudioRecorder*>(userData);
            if (!recorder->m_recording)
                return paComplete;

            // Copy data from input buffer to our recording buffer
            auto* samples = static_cast<const int16_t*>(inputBuffer);
            recorder->m_recordedSamples.insert(recorder->m_recordedSamples.end(), samples, samples + framesPerBuffer);

            return paContinue;
        };

        // Open the stream
        err = Pa_OpenStream(&m_stream,
                            &inputParams,
                            nullptr,  // No output
                            sampleRate,
                            framesPerBuffer,
                            paClipOff,
                            callback,
                            this);

        if (err != paNoError) {
            spdlog::error("Failed to open audio stream: {}", Pa_GetErrorText(err));
            return false;
        }

        // Start recording
        err = Pa_StartStream(m_stream);
        if (err != paNoError) {
            spdlog::error("Failed to start audio stream: {}", Pa_GetErrorText(err));
            Pa_CloseStream(m_stream);
            return false;
        }

        // Record for specified duration
        spdlog::info("Recording for {} seconds...", durationSec);
        Pa_Sleep(durationSec * 1000);

        // Stop recording
        m_recording = false;
        Pa_StopStream(m_stream);
        Pa_CloseStream(m_stream);

        // Write audio data to file
        outFile.write(reinterpret_cast<const char*>(m_recordedSamples.data()), m_recordedSamples.size() * sizeof(int16_t));

        // Update WAV header with correct size
        updateWavHeader(outFile, m_recordedSamples.size() * sizeof(int16_t));

        spdlog::info("Recording saved to {}", filePath);
        return true;
    }

   private:
    void writeWavHeader(std::ofstream& file, int channels, int sampleRate, int bitDepth) {
        // Standard WAV header
        // RIFF chunk
        file.write("RIFF", 4);
        file.write("\0\0\0\0", 4);  // Size (filled in later)
        file.write("WAVE", 4);

        // fmt chunk
        file.write("fmt ", 4);
        int32_t fmtSize = 16;
        file.write(reinterpret_cast<const char*>(&fmtSize), 4);
        int16_t audioFormat = 1;  // PCM
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
        file.write("\0\0\0\0", 4);  // Size (filled in later)
    }

    void updateWavHeader(std::ofstream& file, size_t dataSize) {
        file.seekp(4);
        int32_t riffSize = static_cast<int32_t>(dataSize + 36);  // RIFF chunk size
        file.write(reinterpret_cast<const char*>(&riffSize), 4);

        file.seekp(40);
        int32_t dataChunkSize = static_cast<int32_t>(dataSize);
        file.write(reinterpret_cast<const char*>(&dataChunkSize), 4);
    }

    bool m_initialized;
    bool m_recording;
    PaStream* m_stream;
    std::vector<int16_t> m_recordedSamples;
};
#include "../../include/Audio/AudioManager.h"

AudioManager::AudioManager() = default;

AudioManager::~AudioManager() {
    delete audioManager;
}

AudioManager* AudioManager::GetInstance() {
    return (audioManager == nullptr) ? audioManager = new AudioManager() : audioManager;
}

void AudioManager::InitializeAudio() {
#ifdef DEBUG
    ZoneScopedNC("Audio manager", 0xDC143C);
#endif
    audioDevice = alcOpenDevice(nullptr);

    if (audioDevice) {
        audioContext = alcCreateContext(audioDevice, nullptr);
        alcMakeContextCurrent(audioContext);
    }

    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
    alDisable(AL_STOP_SOURCES_ON_DISCONNECT_SOFT);

    spdlog::info("Successfully initialized OpenAL-Soft on " +
                 (std::string)alcGetString(audioDevice, ALC_ALL_DEVICES_SPECIFIER));

    deviceChanger = std::jthread(CheckAudioDevice, &audioDevice);
}

void AudioManager::CheckAudioDevices() {
    // Get the list of available audio devices
    const ALCchar* deviceList = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
    if (deviceList == nullptr) {
        spdlog::error("Failed to retrieve audio device list.");
        return;
    }

    spdlog::info("Available audio devices:");

    // Iterate over the device list until finding a null terminator
    while (*deviceList != '\0') {
        spdlog::info("- {}", deviceList);
        // Move the pointer to the next device name
        deviceList += strlen(deviceList) + 1;
    }
}

void AudioManager::InitializeAudioDevice(const char* deviceName) {
    // Open the specified audio device
    ALCdevice* audioDevice = alcOpenDevice(deviceName);
    if (audioDevice == nullptr) {
        spdlog::error("Failed to open audio device: {}", deviceName);
        return;
    }

    // Create an audio context
    ALCcontext* audioContext = alcCreateContext(audioDevice, nullptr);
    if (audioContext == nullptr) {
        spdlog::error("Failed to create audio context.");
        alcCloseDevice(audioDevice);
        return;
    }

    // Make the audio context current
    if (!alcMakeContextCurrent(audioContext)) {
        spdlog::error("Failed to make audio context current.");
        alcDestroyContext(audioContext);
        alcCloseDevice(audioDevice);
        return;
    }

    // Print information about the audio device
    spdlog::info("Successfully initialized audio device: {}", alcGetString(audioDevice, ALC_ALL_DEVICES_SPECIFIER));

    // Clean up: Destroy the audio context and close the audio device
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(audioContext);
    alcCloseDevice(audioDevice);
}

void AudioManager::RemoveAudioSource(int componentId) {
    if (audioSources.contains(componentId)) {
        audioSources.at(componentId)->Free();
        audioSources.erase(componentId);
    }
}

void AudioManager::Free() {
    deviceChanger.request_stop();
    deviceChanger.join();

    for (auto&& source: audioSources) {
        source.second->Free();
    }

    audioSources.clear();

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(audioContext);
    alcCloseDevice(audioDevice);

    audioDevice = nullptr;
    audioContext = nullptr;
}

void AudioManager::CheckAudioDevice(const std::stop_token& token, ALCdevice** device) {
    using namespace std::chrono;

    ALCint status;
    static auto alcReopenDeviceSOFT = (LPALCREOPENDEVICESOFT)alcGetProcAddress(*device, "alcReopenDeviceSOFT");
    static auto alcResetDeviceSOFT = (LPALCRESETDEVICESOFT)alcGetProcAddress(*device, "alcResetDeviceSOFT");

    while(!token.stop_requested()) {
        alcGetIntegerv(*device, ALC_CONNECTED, 1, &status);

        if (!status)
        {
            spdlog::error("Audio device disconnected! Trying to reconnect...");

            if (!alcResetDeviceSOFT(*device, nullptr))
            {
                spdlog::error("Audio device reconnecting failed! Trying to open new device...");

                if (!alcReopenDeviceSOFT(*device, nullptr, nullptr)) {
                    spdlog::error("Opening new audio device failed! Trying again in 500 ms...");
                } else {
                    spdlog::info("Successfully opened new device on " +
                                 (std::string)alcGetString(*device, ALC_ALL_DEVICES_SPECIFIER));
                }
            } else {
                spdlog::info("Successfully reconnected OpenAL-Soft on " +
                             (std::string)alcGetString(*device, ALC_ALL_DEVICES_SPECIFIER));
            }
        }

        std::this_thread::sleep_for(500ms);
    }
}
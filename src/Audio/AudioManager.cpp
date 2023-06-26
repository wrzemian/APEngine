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

void AudioManager::CreateAll(Camera camera, Object3D player) {
    auto listener = std::make_shared<AudioListener>(camera, 0);
    listener->Start();
    listener->OnCreate();
    listener->SetGain(1.0f);

    auto source = std::make_shared<AudioSource>(player, 0);
    source->Start();
    source->OnCreate();
    source->LoadAudioData("../../res/audio/crank_jump.wav", AudioType::Direct);
    source->IsLooping(false);
    source->SetGain(1.0f);
    auto source2 = std::make_shared<AudioSource>(player, 1);
    source2->Start();
    source2->OnCreate();
    source2->LoadAudioData("../../res/audio/crank_falling.wav", AudioType::Direct);
    source2->IsLooping(false);
    source2->SetGain(1.0f);
    auto source3 = std::make_shared<AudioSource>(player, 2);
    source3->Start();
    source3->OnCreate();
    source3->LoadAudioData("../../res/audio/crank_movement1.wav", AudioType::Direct);
    source3->IsLooping(true);
    source3->SetGain(1.0f);
    auto source4 = std::make_shared<AudioSource>(player, 3);
    source4->Start();
    source4->OnCreate();
    source4->LoadAudioData("../../res/audio/jetpack.wav", AudioType::Direct);
    source4->IsLooping(false);
    source4->SetGain(1.0f);
    auto source5 = std::make_shared<AudioSource>(player, 4);
    source5->Start();
    source5->OnCreate();
    source5->LoadAudioData("../../res/audio/michel_falling1.wav", AudioType::Direct);
    source5->IsLooping(true);
    source5->SetGain(1.0f);
    auto source6 = std::make_shared<AudioSource>(player, 5);
    source6->Start();
    source6->OnCreate();
    source6->LoadAudioData("../../res/audio/michel_steps.wav", AudioType::Direct);
    source6->IsLooping(false);
    source6->SetGain(1.0f);
    auto source7 = std::make_shared<AudioSource>(player, 6);
    source7->Start();
    source7->OnCreate();
    source7->LoadAudioData("../../res/audio/chest_falling.wav", AudioType::Direct);
    source7->IsLooping(false);
    source7->SetGain(1.0f);
    auto source8 = std::make_shared<AudioSource>(player, 7);
    source8->Start();
    source8->OnCreate();
    source8->LoadAudioData("../../res/audio/platforms.wav", AudioType::Direct);
    source8->IsLooping(true);
    source8->SetGain(1.0f);
    auto source9 = std::make_shared<AudioSource>(player, 8);
    source9->Start();
    source9->OnCreate();
    source9->LoadAudioData("../../res/audio/train.wav", AudioType::Direct);
    source9->IsLooping(true);
    source9->SetGain(1.0f);

}

void AudioManager::PlaySound(Audio name) {
    audioSources.at(name)->PlaySound();
}

void AudioManager::PauseSound(Audio name) {
    audioSources.at(name)->StopSound();
}

void AudioManager::Update() {
    audioListener->Update();
    for (auto& it: audioSources) {
        it.second->Update();
    }
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
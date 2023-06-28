#ifndef APENGINE_AUDIOMANAGER_H
#define APENGINE_AUDIOMANAGER_H

#include <al.h>
#include <alc.h>
#include <alext.h>
#include "spdlog/spdlog.h"
#include "AudioListener.h"
#include "AudioSource.h"
#include <memory>
#include <unordered_map>

#define AL_STOP_SOURCES_ON_DISCONNECT_SOFT 0x19AB

enum Audio {
    CRANK_JUMP, CRANK_LAND, CRANK_MOVE,
    MICHEL_JUMP, MICHEL_LAND, MICHEL_WALK,
    CHEST_LAND, PLATFORM_MOVE, TRAIN_AMBIENT, JETPACK
};

class AudioManager {

    ALCdevice* audioDevice = nullptr;
    ALCcontext* audioContext = nullptr;
    std::jthread deviceChanger;

    inline static AudioManager* audioManager;
    explicit AudioManager();
    static void CheckAudioDevice(const std::stop_token& token, ALCdevice** device);
public:
    std::shared_ptr<AudioListener> audioListener;
    std::unordered_map<int, std::shared_ptr<AudioSource>> audioSources;

//    Currently unused, maybe in future for audio streaming
//    std::unordered_map<ALuint, std::shared_ptr<AudioSource>> audioBuffers;

    AudioManager(AudioManager &other) = delete;
    void operator=(const AudioManager&) = delete;
    virtual ~AudioManager();

    static AudioManager* GetInstance();
    void Free();

    void InitializeAudio();
    void CreateAll(Camera camera, Object3D player);
    void PlaySound(Audio name);
    void PauseSound(Audio name);
    void Update();
    void RemoveAudioSource(int componentId);
};


#endif //APENGINE_AUDIOMANAGER_H

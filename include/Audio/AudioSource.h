//
// Created by wrzos on 14.06.2023.
//

#ifndef APENGINE_AUDIOSOURCE_H
#define APENGINE_AUDIOSOURCE_H

#include "AudioLoader.h"
#include "AudioListener.h"
#include <al.h>
#include "../Objects/Object3D.h"

constexpr int NUM_BUFFERS = 4;

enum AudioDistanceMode {
    Continuous, Paused
};

class AudioSource : public std::enable_shared_from_this<AudioSource> {
public:

    //player info
    Object3D* parent;
    glm::vec3 position = {};
    glm::vec3 positionOffset = {};
    // Audio data
    int id = -1;
    std::shared_ptr<AudioLoader> audioLoader = nullptr;
    ALuint buffersIds[NUM_BUFFERS] {};
    ALuint sourceId = 0;
    ALint currentState = 0;
    bool isLooping = false;
    bool isPlaying = false;
    bool startWithTheGame = false;
    // Settings
    float gain = 1.0f;
    float maxDistance = 0.0f;
    bool isMovingTarget = false;
    AudioDistanceMode distanceMode = AudioDistanceMode::Continuous;

    AudioSource(Object3D &parent, int id);

    ~AudioSource();

    void Start();

    void Update();

    void OnCreate();

    void OnDestroy();

    void Free();

    void LoadAudioData(const char* path, AudioType type);

    void PlaySound() const;
    void ForcePlaySound() const;
    void PauseSound() const;
    void StopSound() const;
    void SetPositionOffset(glm::vec3 offset);
    void SetDistanceMode(AudioDistanceMode mode);
    void SetGain(float val);
    void SetPitch(float val) const;
    void SetVelocity(glm::vec3 velocity) const;
    void SetMaxDistance(float val);
    void IsLooping(bool state);
    void IsMoving(bool state);
    void SetCone(glm::vec3 direction, glm::vec2 cone) const;
    const float GetGain() const;
};


#endif //APENGINE_AUDIOSOURCE_H

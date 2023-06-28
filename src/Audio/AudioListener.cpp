#include "../../include/Audio/AudioListener.h"
#include "../../include/Audio/AudioManager.h"
#include <memory>

AudioListener::AudioListener(Camera &camera, int id) {
    this->parent = &camera;
    this->id = id;
}

AudioListener::~AudioListener() = default;

void AudioListener::Start() {
    position = parent->Position;

    alListenerf(AL_GAIN, gain);
    alListener3f(AL_POSITION, position.x, position.y, position.z);
    alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);

    ALfloat listenerOrientation[] = {0.0, 0.0, -1.0,  0.0, 1.0, 0.0};
    alListenerfv(AL_ORIENTATION,listenerOrientation);
}

void AudioListener::Update() {
    position = parent->Position;
    alListener3f(AL_POSITION, position.x, position.y, position.z);
}

void AudioListener::OnCreate() {
    AudioManager::GetInstance()->audioListener = shared_from_this();
}

void AudioListener::SetGain(float val) {
    if (val < 0.0f) {
        gain = 0.0f;
    } else {
        gain = val;
    }

    alListenerf(AL_GAIN, gain);
}

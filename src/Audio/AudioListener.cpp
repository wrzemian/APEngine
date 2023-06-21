#include "../../include/Audio/AudioListener.h"
#include "../../include/Audio/AudioManager.h"
#include <memory>

AudioListener::AudioListener(Camera &camera, int id) {
    this->parent = &camera;
    this->id = id;
   // this->sharedPtr = std::make_shared<AudioListener>(this);
}

AudioListener::~AudioListener() = default;

void AudioListener::Start() {
    position = parent->Position;

    alListenerf(AL_GAIN, gain);
    alListener3f(AL_POSITION, position.x, position.y, position.z);
    alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);

    // "up" and "at" vectors, i.e. at the head of the player
    ////TO CHANGE IN CASE OF PROBLEMS
    ALfloat listenerOrientation[] = {0.0, 0.0, -1.0,  0.0, 1.0, 0.0};
    alListenerfv(AL_ORIENTATION,listenerOrientation);
}

void AudioListener::Update() {
#ifdef DEBUG
    ZoneScopedNC("Audio listener", 0x800080);
#endif
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

// Return current gain value
const float AudioListener::GetGain() const {
    return gain;
}
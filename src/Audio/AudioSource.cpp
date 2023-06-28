#include "../../include/Audio/AudioSource.h"
#include "../../include/Audio/AudioManager.h"

AudioSource::AudioSource(Object3D &parent, int id) {
    this->parent = &parent;
    this->id = id;
}

AudioSource::~AudioSource() = default;

void AudioSource::Start() {
    if (startWithTheGame)
        alSourcePlay(sourceId);
}

void AudioSource::Update() {
    if (isMovingTarget) {
        position = parent->_transform._position + positionOffset;
        alSource3f(sourceId, AL_POSITION, position.x, position.y, position.z);
    }
    alGetSourcei(sourceId, AL_SOURCE_STATE, &currentState);

}

void AudioSource::OnCreate() {
    AudioManager::GetInstance()->audioSources.insert({id, shared_from_this()});
}

void AudioSource::OnDestroy() {
    AudioManager::GetInstance()->RemoveAudioSource(id);
    parent = nullptr;
}

void AudioSource::Free() {
    alSourceStop(sourceId);
    alSourcei(sourceId, AL_BUFFER, NULL);
    alDeleteBuffers(4, buffersIds);
    alDeleteSources(1, &sourceId);
}

void AudioSource::LoadAudioData(const char* path, AudioType type) {
    std::unique_ptr<AudioLoader> loader = std::make_unique<AudioLoader>();

    alGenBuffers(1, buffersIds);

    //  Currently unused, maybe in future for audio streaming
    //  AudioManager::GetInstance()->audioBuffers.insert({bufferId, std::dynamic_pointer_cast<AudioSource>(shared_from_this())});

    loader->LoadAudioFile(path, type, *buffersIds);

    alGenSources(1, &sourceId);

    alSourcef(sourceId, AL_PITCH, 1);
    alSourcef(sourceId, AL_GAIN, gain);

    if (type == AudioType::Positional) {
        position = parent->_transform._position + positionOffset;

        alSource3f(sourceId, AL_POSITION, position.x, position.y, position.z);
        alSource3f(sourceId, AL_VELOCITY, 0, 0, 0);
        alSourcef(sourceId, AL_ROLLOFF_FACTOR, 1.5f);
    }

    alSourcei(sourceId, AL_LOOPING, AL_FALSE);
    alSourcei(sourceId, AL_BUFFER, *buffersIds);
}

void AudioSource::PlaySound() const {
    if (currentState != AL_PLAYING) {
        alSourcePlay(sourceId);
    }
}

void AudioSource::PauseSound() const {
    if (currentState == AL_PLAYING) {
        alSourcePause(sourceId);
    }
}

void AudioSource::StopSound() const {
    if (currentState == AL_PLAYING) {
        alSourceStop(sourceId);
    }
}

void AudioSource::SetPositionOffset(glm::vec3 offset) {
    positionOffset = offset;
    position += positionOffset;
    alSource3f(sourceId, AL_POSITION, position.x, position.y, position.z);
}

void AudioSource::SetDistanceMode(AudioDistanceMode mode) {
    distanceMode = mode;
}

void AudioSource::SetGain(float val) {
    if (val < 0.0f) {
        gain = 0.0f;
    } else {
        gain = val;
    }

    alSourcef(sourceId, AL_GAIN, val);
}

void AudioSource::SetMaxDistance(float val) {
    maxDistance = val;

    if (maxDistance == 0.0f) {
        alSourcef(sourceId, AL_ROLLOFF_FACTOR, 1.5f);
        alSourcef(sourceId, AL_MAX_DISTANCE, maxDistance);
    } else if (maxDistance > 0.0f) {
        alSourcef(sourceId, AL_ROLLOFF_FACTOR,
                  gain * (maxDistance * 2.0f) / (1.5f + gain * 2.0f));
        alSourcef(sourceId, AL_MAX_DISTANCE, maxDistance);
    }
}

void AudioSource::IsLooping(bool state) {
    alSourcei(sourceId, AL_LOOPING, state);
}

void AudioSource::IsMoving(bool state) {
    isMovingTarget = state;
}

void AudioSource::SetCone(glm::vec3 direction, glm::vec2 cone) const {
    alSource3f(sourceId, AL_DIRECTION, direction.x, direction.y, direction.z);
    alSourcef(sourceId, AL_CONE_INNER_ANGLE, cone.x);
    alSourcef(sourceId, AL_CONE_OUTER_ANGLE, cone.y);
}
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
#ifdef DEBUG
    ZoneScopedNC("Audio source", 0x800080);
#endif
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

/**
 * @annotation
 * Plays the sound despite its current status.
 */
void AudioSource::ForcePlaySound() const {
    alSourcePlay(sourceId);
}

/**
 * @annotation
 * Pauses the sound.
 */
void AudioSource::PauseSound() const {
    if (currentState == AL_PLAYING) {
        alSourcePause(sourceId);
    }
}

/**
 * @annotation
 * Stops the sound.
 */
void AudioSource::StopSound() const {
    if (currentState == AL_PLAYING) {
        alSourceStop(sourceId);
    }
}

/**
 * @annotation
 * Sets position offset.
 * @param offset - vector3 added to position, default {0}
 */
void AudioSource::SetPositionOffset(glm::vec3 offset) {
    positionOffset = offset;
    position += positionOffset;
    alSource3f(sourceId, AL_POSITION, position.x, position.y, position.z);
}

void AudioSource::SetDistanceMode(AudioDistanceMode mode) {
    distanceMode = mode;
}

/**
 * @annotation
 * Sets audio pitch.
 * @param val - [0.5 - 2.0], default 1.0
 */
void AudioSource::SetPitch(float val) const {
    alSourcef(sourceId, AL_PITCH, val);
}

/**
 * @annotation
 * Sets audio gain. Each division/multiply by 2 results in -6/+6 dB.
 * @param val - [0.0 - x], default 1.0.
 */
void AudioSource::SetGain(float val) {
    if (val < 0.0f) {
        gain = 0.0f;
    } else {
        gain = val;
    }

    alSourcef(sourceId, AL_GAIN, val);
}

/**
 * @annotation
 * Sets audio velocity. Used in calculating doppler shift (moving objects emitting sound).
 * @param velocity - vector3, default {0}
 */
void AudioSource::SetVelocity(glm::vec3 velocity) const {
    alSource3f(sourceId, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

/**
 * @annotation
 * Sets max distance for playing audio. Audio volume will be adjusted to better suit new distance
 * for more realistic effect.
 * @param val - [0.0f - x], default 0.0f
 */
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

/**
 * @annotation
 * Sets audio looping state.
 * @param state - true or false, default false
 */
void AudioSource::IsLooping(bool state) {
    alSourcei(sourceId, AL_LOOPING, state);
}

/**
 * @annotation
 * Sets the target moving state. If set to true, then the position will be updated every time target moves.
 * @param state - true or false, default false
 */
// Sets if the target will be moving. If set to true, then the position
void AudioSource::IsMoving(bool state) {
    isMovingTarget = state;
}

/**
 * @annotation
 * Sets audio cone settings.
 * @param direction - vector3, sound direction
 * @param cone - vector2, inner angle and outer angle
 */
void AudioSource::SetCone(glm::vec3 direction, glm::vec2 cone) const {
    alSource3f(sourceId, AL_DIRECTION, direction.x, direction.y, direction.z);
    alSourcef(sourceId, AL_CONE_INNER_ANGLE, cone.x);
    alSourcef(sourceId, AL_CONE_OUTER_ANGLE, cone.y);
}
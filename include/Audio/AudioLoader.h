#ifndef APENGINE_AUDIOLOADER_H
#define APENGINE_AUDIOLOADER_H

#include <spdlog/spdlog.h>
#include <AudioFile.h>
#include <al.h>

enum AudioType {
    Positional, Direct
};

class AudioLoader {

public:
    ~AudioLoader();


    AudioLoader();

    void LoadAudioFile(const char* path, AudioType type, ALuint bufferId);
};


#endif //APENGINE_AUDIOLOADER_H

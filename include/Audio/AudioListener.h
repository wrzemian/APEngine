#ifndef APENGINE_AUDIOLISTENER_H
#define APENGINE_AUDIOLISTENER_H

#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <al.h>
#include "../Objects/Object3D.h"
#include "../Camera.h"

class AudioListener : public std::enable_shared_from_this<AudioListener> {
public:
    std::shared_ptr<AudioListener> sharedPtr = nullptr;
    Camera* parent;
    glm::vec3 position = {};
    glm::vec3 velocity = {};
    float gain = 1.0f;
    int id;

    AudioListener(Camera &camera, int id);

    ~AudioListener();

    void Start();

    void Update();

    void OnCreate();

    void SetGain(float val);

};


#endif //APENGINE_AUDIOLISTENER_H

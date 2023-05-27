//
// Created by Karol on 23.05.2023.
//

#ifndef APENGINE_BUTTON_H
#define APENGINE_BUTTON_H

#include "Object3D.h"
#include "Platform.h"

class Button : public Object3D {
private:
    using super = Object3D;

    bool isPushed = false;
    float t = 1.0f;
    float timeToReset = 1.0f;
public:
    Button(Platform* p, glm::vec3 position);

    void Update(float dt);
    virtual void onCollision(Object3D* other);

    Platform* connectedPlatform;

    void logFields();
};


#endif //APENGINE_BUTTON_H

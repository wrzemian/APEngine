//
// Created by Karol on 22.05.2023.
//

#ifndef APENGINE_PLATFORM_H
#define APENGINE_PLATFORM_H

#include "Object3D.h"
#include "Hitbox.h"
#include "MovingObject.h"

class Platform : public MovingObject {
private:
    typedef Object3D super;

public:

    bool moveToTarget = false;
    bool moveToOrigin = false;
    float t;
    float speed = 1; //1 equals to one second of movement
    int targetId = -1;
    glm::vec3 positionOrigin = glm::vec3(0);
    glm::vec3 positionTarget = glm::vec3(0);
    Platform(glm::vec3 pOrigin,glm::vec3 pTarget, float s);
    void onCollision(Object3D* other) override;
    void Move(float deltaTime) override;
    void OnActivate();
    void OnDeactivate();

    void logFields();

    void ImGui() override;
};


#endif //APENGINE_PLATFORM_H

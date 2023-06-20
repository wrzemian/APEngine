//
// Created by Karol on 28.05.2023.
//

#ifndef APENGINE_GRABBER_H
#define APENGINE_GRABBER_H

#include "MovingObject.h"
#include "Box.h"

class Grabber : public MovingObject {
private:

    bool moveToTarget = false;
    bool moveToOrigin = false;
    float t;
    float speed = 1.0f; //1 equals to one second of movement
    float forwardSpeed = 1.0f;
    float backSpeed = 3.0f;
    glm::vec3 positionOrigin = glm::vec3(0);
    glm::vec3 positionTarget = glm::vec3(3,3,0);
    glm::vec3 playerOffset = {};
    glm::vec3 grabbedObjectOffset = glm::vec3(0);
    glm::vec3 playerPos = glm::vec3(0);
    glm::vec3 playerRot = glm::vec3(0);
    bool grabbed = false;
    Object3D* grabbedBox;
public:
    Grabber();
    void onCollision(Object3D* other) override;
    void Move(float deltaTime) override;
    void UpdateGrabber(glm::vec3 pos,glm::vec3 rot);
    void Grab();
    void SetPlayerOffset(glm::vec3 offset);
    void SetPositionTarget(glm::vec3 targetPos);
    void DropObject();
    void LoadAnimations();
    Animation stamdA, jumpA, hookingA, walkA;
};


#endif //APENGINE_GRABBER_H

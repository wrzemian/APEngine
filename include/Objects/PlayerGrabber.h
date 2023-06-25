//
// Created by Karol on 25.05.2023.
//

#ifndef APENGINE_PLAYERGRABBER_H
#define APENGINE_PLAYERGRABBER_H

#include "MovingObject.h"
#include "../include/Engine.h"
#include "../include/User/InputSystem.h"
#include "Battery.h"
#include "Grabber.h"

class PlayerGrabber: public MovingObject {
private:

    using super = MovingObject;
    int jumpCount = 0;
    bool haveBattery = false;
    bool canPickUpBattery = false;
    int walking;
    int recentlyMoved;

    glm::vec3 batteryOffset = glm::vec3(-0.25f, 1.0f, 0.0f);
    glm::vec3 direction =  glm::vec3(0.0f, 0.0f, 0.0f);
    float padJoystickTolerance = 0.2f;
public:

    Animation walkP, standP, jumpP, hookingP;
    PlayerGrabber();
    Battery* battery;
    Grabber* grabber;
    void initPlayer(InputSystem* inputSystem);
    void UpdatePlayer(InputSystem* inputSystem, float movementSpeed);
    void onCollision(Object3D* other) override;
    void onCollisionExit(Object3D* other) override;
    void onCollisionY(Object3D *other) override;
    void unusualCollision(Object3D *other) override;
    void switchAnimationWalk();
    void switchAnimationJump();
    void switchAnimationStand();
    void switchAnimationGrab();
    void Jump();
    void Grab();
    void loadAnimations();
    void DropBattery();
};


#endif //APENGINE_PLAYERGRABBER_H

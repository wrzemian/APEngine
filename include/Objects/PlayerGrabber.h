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
    glm::vec3 batteryOffset = glm::vec3(-0.2f, 0.2f, 0.0f);
public:
    PlayerGrabber();
    Battery* battery;
    Grabber* grabber;
    void initPlayer(InputSystem* inputSystem);
    void UpdatePlayer(InputSystem* inputSystem, float movementSpeed);
    void onCollision(Object3D* other) override;
    void onCollisionExit(Object3D* other) override;
    void switchAnimationWalk();
    void switchAnimationJump();
    void switchAnimationStand();
    void Jump();
    void Grab();
};


#endif //APENGINE_PLAYERGRABBER_H

//
// Created by Karol on 23.05.2023.
//

#ifndef APENGINE_PLAYERJUMPER_H
#define APENGINE_PLAYERJUMPER_H

#include "MovingObject.h"
#include "../include/Engine.h"
#include "../include/User/InputSystem.h"
#include "Battery.h"

class PlayerJumper : public MovingObject {
private:
    using super = MovingObject;
    int jumpCount = 0;
    bool haveBattery = false;
    bool canPickUpBattery = false;
    glm::vec3 batteryOffset = glm::vec3(-0.2f, 0.2f, 0.0f);

    glm::vec3 direction =  glm::vec3(0.0f, 0.0f, 0.0f);

    int walking;
    int recentlyMoved;

    float padJoystickTolerance = 0.2f;

public:
    Battery* battery;
    void initPlayer(InputSystem* inputSystem);
    void UpdatePlayer(InputSystem* inputSystem, float movementSpeed);
    void Jump();
    void onCollision(Object3D* other) override;
    void onCollisionExit(Object3D* other) override;
    void onCollisionY(Object3D *other) override;
    void unusualCollision(Object3D *other) override;
    void switchAnimationWalk();
    void switchAnimationJump();
    void switchAnimationStand();
    void loadAnimations();
};


#endif //APENGINE_PLAYERJUMPER_H

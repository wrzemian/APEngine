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
    Animation walkA, stamdA, jumpA;
    using super = MovingObject;
    int jumpCount = 0;
    bool haveBattery = false;
    bool canPickUpBattery = false;
    glm::vec3 batteryOffset = glm::vec3(-0.25f, 1.0f, 0.0f);

    glm::vec3 boxOffset = glm::vec3(0.7f, 0.45f, 0.0f);

    glm::vec3 direction =  glm::vec3(0.0f, 0.0f, 0.0f);

    int walking;
    int recentlyMoved;
    int recentlyJumped = 0;

    float padJoystickTolerance = 0.2f;

public:
    Object3D* lastTouchedBox;
    bool pickedUpBox = false;

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
    void DropBattery();
};


#endif //APENGINE_PLAYERJUMPER_H

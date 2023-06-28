//
// Created by micha on 02.04.2023.
//

#pragma once

#include "Object3D.h"
#include "random"

class MovingObject : public Object3D {

private:
    using super = Object3D;

public:
    MovingObject();

    virtual ~MovingObject();

    glm::vec3 _velocity = glm::vec3(0, 0, 0);
    glm::vec3 _gravity = glm::vec3(0, -10, 0);

    void SetVelocity(glm::vec3 velocity);
    void AddVelocity(glm::vec3 velocity);
    void StopMoving();

    virtual void Move(float deltaTime);

    void onCollision(Object3D* other) override;

    rapidjson::Document ParseToJSON();

    void loadFromJSON(MovingObject temp);

    void onCollisionX(Object3D *other) override;
    void onCollisionY(Object3D *other) override;
    void onCollisionZ(Object3D *other) override;

    void SwitchGravity(bool state) override;
};



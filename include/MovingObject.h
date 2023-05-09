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

    glm::vec3 _velocity;

    void SetVelocity(glm::vec3 velocity);
    void AddVelocity(glm::vec3 velocity);
    void StopMoving();
    void Move();

    void ImGui();
    void onCollision(Object3D* other) override;

    rapidjson::Document ParseToJSON();

    void loadFromJSON(MovingObject temp);
};



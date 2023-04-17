//
// Created by micha on 02.04.2023.
//

#pragma once

#include "Object3D.h"

class MovingObject : public Object3D {

private:
    typedef Object3D super;

public:
    MovingObject();

    virtual ~MovingObject();

    glm::vec3 _velocity;

    void SetVelocity(glm::vec3 velocity);
    void AddVelocity(glm::vec3 velocity);
    void StopMoving();
    void Move();

    void ImGui();
};



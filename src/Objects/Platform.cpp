//
// Created by Karol on 22.05.2023.
//

#include "../../include/Objects/Platform.h"

Platform::Platform(glm::vec3 pOrigin,glm::vec3 pTarget, float s) {
    this->positionOrigin = pOrigin;
    this->positionTarget = pTarget;
    this->speed = s;
    _transform._position = pOrigin;
}

void Platform::UpdatePosition(float dt) {
    if (moveToTarget) {
        t += dt * speed;
        if (t >= 1.0f) {
            t = 1.0f;
            moveToTarget = false;
        }
    }
    else if(moveToOrigin){
        t -= dt * speed;
        if (t <= 0.0f) {
            t = 0.0f;
            moveToOrigin = false;
        }
    }
    _transform._position = (1.0f - t) * positionOrigin + t * positionTarget;
}

void Platform::OnActivate() {
    moveToTarget = true;
    moveToOrigin = false;
}

void Platform::OnDeactivate() {
    moveToTarget = false;
    moveToOrigin = true;
}

void Platform::onCollision(std::shared_ptr<Object3D> other) {
    //Object3D::onCollision(other);
}

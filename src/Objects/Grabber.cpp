//
// Created by Karol on 28.05.2023.
//

#include "../../include/Objects/Grabber.h"


Grabber::Grabber() {
    tag = "grabber";
    _transform._scale.x  = 0.3f;
    _transform._scale.y  = 0.3f;
    _transform._scale.z  = 0.3f;
}

void Grabber::onCollision(Object3D *other) {
    //MovingObject::onCollision(other);
    if(other->tag == "box")
    {
        std::cout << "hit a box" << std::endl;
    }
    if(other->tag == "box" && !grabbed && moveToTarget)
    {
        grabbedBox = other;
        moveToTarget = false;
        moveToOrigin = true;
        grabbed = true;
    }
}

void Grabber::Move(float deltaTime) {
    //MovingObject::Move(deltaTime);
    if (moveToTarget) {
        t += deltaTime * speed;
        if (t >= 1.0f) {
            t = 1.0f;
            moveToTarget = false;
            moveToOrigin = true;
        }
    }
    else if(moveToOrigin){
        t -= deltaTime * speed;
        if (t <= 0.0f) {
            t = 0.0f;
            moveToOrigin = false;
            DropObject();
        }
    }
    if(grabbed)
    {
        grabbedBox->_transform._position = grabbedObjectOffset + _transform._position;
    }
    _transform._position = (1.0f - t) * (playerPos+positionOrigin+playerOffset) + t * (playerPos+positionTarget+playerOffset);
}

void Grabber::Grab() {
    if(!moveToTarget && !moveToOrigin && !grabbed)
    {
        moveToTarget = true;
    }
}

void Grabber::SetPlayerOffset(glm::vec3 offset) {
    playerOffset = offset;
}

void Grabber::DropObject() {
    grabbed = false;
}

void Grabber::UpdateGrabber(glm::vec3 pos) {
    playerPos = pos;
}

void Grabber::SetPositionTarget(glm::vec3 targetPos) {
    positionTarget = targetPos;
}

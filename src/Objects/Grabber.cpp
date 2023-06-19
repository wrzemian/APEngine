//
// Created by Karol on 28.05.2023.
//

#include "../../include/Objects/Grabber.h"


Grabber::Grabber() {
    IGui::setWindowName("grabber");

    tag = "grabber";
    _transform._scale.x  = 0.3f;
    _transform._scale.y  = 0.3f;
    _transform._scale.z  = 0.3f;
    _gravity.y = 0;
//    LoadAnimations();
}

void Grabber::onCollision(Object3D *other) {
    //MovingObject::onCollision(other);
    if(other->tag == "box" && !grabbed && moveToTarget)
    {
        grabbedBox = other;
        moveToTarget = false;
        moveToOrigin = true;
        grabbed = true;
    }
    else if(other->tag == "floor" || other->tag == "wall" || other->tag == "platform" || other->tag ==  "static platform")
    {
        moveToTarget = false;
        moveToOrigin = true;
    }
}

void Grabber::Move(float deltaTime) {
    //MovingObject::Move(deltaTime);
    if (moveToTarget) {
        t += deltaTime * forwardSpeed;
        if (t >= 1.0f) {
            t = 1.0f;
            moveToTarget = false;
            moveToOrigin = true;
        }
    }
    else if(moveToOrigin){
        t -= deltaTime * backSpeed;
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
    glm::quat playerQuat = glm::quat(playerRot); // Convert Euler angles to quaternion
    glm::mat4 rotationMat = glm::mat4_cast(playerQuat); // Convert quaternion to rotation matrix
    glm::vec3 origin = playerPos + glm::vec3(rotationMat * glm::vec4(positionOrigin+playerOffset, 1.0f));
    glm::vec3 target = playerPos + glm::vec3(rotationMat * glm::vec4(positionTarget+playerOffset, 1.0f));
    _transform._position = (1.0f - t) * origin + t * target;
    _transform._rotation = playerRot;
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

void Grabber::UpdateGrabber(glm::vec3 pos,glm::vec3 rot) {
    playerPos = pos;
    playerRot = rot;
}

void Grabber::SetPositionTarget(glm::vec3 targetPos) {
    positionTarget = targetPos;
}

void Grabber::LoadAnimations() {
    //arm animations
    Animation temp("res/models/Players/Cr4nk/RIGHT_HAND_CRANK_WALKING.dae", &*this->_model);
    Animation temp2("res/models/Players/Cr4nk/RIGHT_HAND_CRANK_JUMPING.dae", &*this->_model);
    Animation temp3("res/models/Players/Cr4nk/RIGHT_HAND_CRANK_STANDING.dae", &*this->_model);
    Animation temp4("res/models/Players/Cr4nk/RIGHT_HAND_CRANK_HOOKING.dae", &*this->_model);

    walkA = temp;
    stamdA = temp3;
    jumpA = temp2;
    hookingA = temp4;
    animator.PlayAnimation(&temp);
}

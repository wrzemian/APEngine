//
// Created by Karol on 25.05.2023.
//

#include "../../include/Objects/PlayerGrabber.h"

void PlayerGrabber::initPlayer(InputSystem* inputSystem) {
    loadFromJSON(Engine::parser.CreateFromJSONMovingObject("objects/movingObj_1"));
    tag = "player";
    inputSystem->monitorKey(GLFW_KEY_UP);
    inputSystem->monitorKey(GLFW_KEY_LEFT);
    inputSystem->monitorKey(GLFW_KEY_RIGHT);
    inputSystem->monitorKey(GLFW_KEY_DOWN);
    inputSystem->monitorKey(GLFW_KEY_KP_2);
    inputSystem->monitorKey(GLFW_KEY_KP_3);
    //inputSystem->monitorKey(GLFW_KEY_SPACE);
}

void PlayerGrabber::UpdatePlayer(InputSystem* inputSystem, float movementSpeed) {
    if (inputSystem->GetKey(GLFW_KEY_UP)) {
        _velocity.z = -movementSpeed;
    }
    else if (inputSystem->GetKey(GLFW_KEY_DOWN)) {
        _velocity.z = movementSpeed;
    }
    else {
        _velocity.z = 0;
    }
    if (inputSystem->GetKey(GLFW_KEY_LEFT)) {
        _velocity.x = -movementSpeed;
    }
    else if (inputSystem->GetKey(GLFW_KEY_RIGHT)) {
        _velocity.x = movementSpeed;
    }
    else {
        _velocity.x = 0;
    }
    if (inputSystem->GetKeyDown(GLFW_KEY_KP_2)) {
        if(haveBattery)
        {
            std::cout<< "drop the battery"<<std::endl;
            haveBattery = false;
            canPickUpBattery= false;
        }
        else if (canPickUpBattery)
        {
            haveBattery = true;
            canPickUpBattery= false;
            std::cout<< "pick up battery"<<std::endl;
        }
    }
    if (inputSystem->GetKeyDown(GLFW_KEY_KP_3)) {
        Grab();
    }
    /* if (inputSystem->GetKeyDown(GLFW_KEY_SPACE)) {
         Jump();
         std::cout<<_velocity.y<<std::endl;
         std::cout<<"help"<<std::endl;
     }*/
    if(haveBattery)
    {
        battery->_transform._position=this->_transform._position+batteryOffset;
        battery->_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    }
    grabber->UpdateGrabber(this->_transform._position);
}

void PlayerGrabber::onCollision(Object3D *other) {
    if((other->tag == "floor" || other->tag == "platform" || other->tag == "moving platform") && _velocity.y != 0)
    {
        _velocity.y = 0;
        jumpCount = 0;
    }
    if(other->tag == "battery")
    {
        canPickUpBattery = true;
    }
}

void PlayerGrabber::Jump() {
    if (jumpCount == 0 && this->_velocity.y <= 0&& this->_velocity.y <= 0)
    {
        this->AddVelocity(glm::vec3(0.0f, 4.0f, 0.0f));
        jumpCount += 1;
    }
}

void PlayerGrabber::Grab() {
    if (haveBattery)
    {
        grabber->Grab();
    }
}

void PlayerGrabber::onCollisionExit(Object3D *other) {
    Object3D::onCollisionExit(other);
    if(other->tag == "battery")
    {
        canPickUpBattery = false;
    }
}

PlayerGrabber::PlayerGrabber() {
    IGui::setWindowName("player grabber");
    //TODO: fix this xddd
    _transform._scale.x = 0.2f;
    _transform._scale.y = 0.2f;
    _transform._scale.z = 0.2f;
}

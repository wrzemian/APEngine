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
        direction.z = -1.0f;
    }
    else if (inputSystem->GetKey(GLFW_KEY_DOWN)) {
        direction.z = 1.0f;
    }
    else
    {
        direction.z = 0.0f;
    }
    if (inputSystem->GetKey(GLFW_KEY_LEFT)) {
        direction.x = -1.0f;
    }
    else if (inputSystem->GetKey(GLFW_KEY_RIGHT)) {
        direction.x = 1.0f;
    }
    else
    {
        direction.x = 0.0f;
    }

    if(direction != glm::vec3(0.0f))
    {
        // normalize the direction vector to have a length of 1
        direction = glm::normalize(direction);
        // multiply by the desired speed
        _velocity.x = direction.x * movementSpeed;
        _velocity.z = direction.z * movementSpeed;

        _transform._rotation.y = -atan2(_velocity.z, _velocity.x);
    }
    else {
        _velocity.x = 0.0f;
        _velocity.z = 0.0f;
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
    grabber->UpdateGrabber(this->_transform._position,this->_transform._rotation);
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
}

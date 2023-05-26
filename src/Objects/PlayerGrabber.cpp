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

    //inputSystem->monitorKey(GLFW_KEY_SPACE);
}

void PlayerGrabber::UpdatePlayer(InputSystem* inputSystem, float movementSpeed) {
    if (inputSystem->GetKey(GLFW_KEY_LEFT)) {
        _velocity.z = -movementSpeed;
    }
    else if (inputSystem->GetKey(GLFW_KEY_RIGHT)) {
        _velocity.z = movementSpeed;
    }
    else {
        _velocity.z = 0;
    }
    if (inputSystem->GetKey(GLFW_KEY_DOWN)) {
        _velocity.x = -movementSpeed;
    }
    else if (inputSystem->GetKey(GLFW_KEY_UP)) {
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
}

void PlayerGrabber::onCollision(Object3D& other) {
    if(other.tag == "floor" && _velocity.y != 0)
    {
        _velocity.y = 0;
        jumpCount = 0;
    }
    if(other.tag == "battery")
    {
        canPickUpBattery = true;
    }
}
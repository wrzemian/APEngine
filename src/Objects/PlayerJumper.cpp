//
// Created by Karol on 23.05.2023.
//

#include "../../include/Objects/PlayerJumper.h"

void PlayerJumper::initPlayer(InputSystem* inputSystem) {
    loadFromJSON(Engine::parser.CreateFromJSONMovingObject("objects/movingObj_0"));
    tag = "player";
    inputSystem->monitorKey(GLFW_KEY_W);
    inputSystem->monitorKey(GLFW_KEY_A);
    inputSystem->monitorKey(GLFW_KEY_S);
    inputSystem->monitorKey(GLFW_KEY_D);
    inputSystem->monitorKey(GLFW_KEY_E);
    //inputSystem->monitorKey(GLFW_KEY_SPACE);
}

void PlayerJumper::UpdatePlayer(InputSystem* inputSystem, float movementSpeed) {
    if (inputSystem->GetKey(GLFW_KEY_W)) {
        _velocity.z = -movementSpeed;
    }
    else if (inputSystem->GetKey(GLFW_KEY_S)) {
        _velocity.z = movementSpeed;
    }
    else {
        _velocity.z = 0;
    }
    if (inputSystem->GetKey(GLFW_KEY_A)) {
        _velocity.x = -movementSpeed;
    }
    else if (inputSystem->GetKey(GLFW_KEY_D)) {
        _velocity.x = movementSpeed;
    }
    else {
        _velocity.x = 0;
    }
    if (inputSystem->GetKeyDown(GLFW_KEY_E)) {
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

void PlayerJumper::Jump() {
    if (jumpCount == 1 && haveBattery)
    {
        //this->AddVelocity(glm::vec3(0.0f, 5.0f, 0.0f));
        this->_velocity.y = 5.0f;
        jumpCount += 1;
    }
    else if (jumpCount == 0)
    {
        //this->AddVelocity(glm::vec3(0.0f, 5.0f, 0.0f));
        this->_velocity.y = 5.0f;
        jumpCount += 1;
    }
}

void PlayerJumper::onCollision(Object3D *other) {
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

void PlayerJumper::onCollisionExit(Object3D *other) {
    Object3D::onCollisionExit(other);
    if(other->tag == "battery")
    {
        canPickUpBattery = false;
    }
}

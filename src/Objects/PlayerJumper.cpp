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
        direction.z = -1.0f;
    }
    else if (inputSystem->GetKey(GLFW_KEY_S)) {
        direction.z = 1.0f;
    }
    else {
        direction.z = 0.0f;
    }
    if (inputSystem->GetKey(GLFW_KEY_A)) {
        direction.x = -1.0f;
    }
    else if (inputSystem->GetKey(GLFW_KEY_D)) {
        direction.x = 1.0f;
    }
    else {
        direction.x = 0.0f;
    }
    if(inputSystem->getJoystickAxis(1, GLFW_GAMEPAD_AXIS_LEFT_X) != 0)
    {
        direction.z = inputSystem->getJoystickAxis(1, GLFW_GAMEPAD_AXIS_LEFT_X);
    }
    if(inputSystem->getJoystickAxis(1, GLFW_GAMEPAD_AXIS_LEFT_Y) != 0)
    {
        direction.x = inputSystem->getJoystickAxis(1, GLFW_GAMEPAD_AXIS_LEFT_Y);
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

    if (inputSystem->GetKeyDown(GLFW_KEY_E) || inputSystem->GetGamepadButtonDown(1, GLFW_GAMEPAD_BUTTON_X)) {
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
        this->_velocity.y = 4.0f;
        jumpCount += 1;
    }
    else if (jumpCount == 0 && this->_velocity.y <= 0)
    {
        //this->AddVelocity(glm::vec3(0.0f, 5.0f, 0.0f));
        this->_velocity.y = 4.0f;
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

//
// Created by Karol on 23.05.2023.
//

#include "../../include/Objects/PlayerJumper.h"

Animation walkA, stamdA, jumpA;

void PlayerJumper::initPlayer(InputSystem* inputSystem) {
    loadFromJSON(Engine::parser.CreateFromJSONMovingObject("objects/movingObj_0"));
    tag = "player";
    inputSystem->monitorKey(GLFW_KEY_W);
    inputSystem->monitorKey(GLFW_KEY_A);
    inputSystem->monitorKey(GLFW_KEY_S);
    inputSystem->monitorKey(GLFW_KEY_D);
    inputSystem->monitorKey(GLFW_KEY_E);
    //inputSystem->monitorKey(GLFW_KEY_SPACE);

    loadAnimations();
}

void PlayerJumper::UpdatePlayer(InputSystem* inputSystem, float movementSpeed) {
    if (inputSystem->GetKey(GLFW_KEY_W)) {
        direction.z = -1.0f;
    }
    else if (inputSystem->GetKey(GLFW_KEY_S)) {
        direction.z = 1.0f;
    }
    else if(inputSystem->getJoystickAxis(1, GLFW_GAMEPAD_AXIS_LEFT_Y) >= padJoystickTolerance || inputSystem->getJoystickAxis(1, GLFW_GAMEPAD_AXIS_LEFT_Y) <= -padJoystickTolerance)
    {
        direction.z = inputSystem->getJoystickAxis(1, GLFW_GAMEPAD_AXIS_LEFT_Y);
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
    else if(inputSystem->getJoystickAxis(1, GLFW_GAMEPAD_AXIS_LEFT_X) >= padJoystickTolerance || inputSystem->getJoystickAxis(1, GLFW_GAMEPAD_AXIS_LEFT_X) <= -padJoystickTolerance)
    {
        direction.x = inputSystem->getJoystickAxis(1, GLFW_GAMEPAD_AXIS_LEFT_X);
    }
    else {
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
        this->switchAnimationJump();
        walking = 0;
        recentlyMoved = 0;
        //this->AddVelocity(glm::vec3(0.0f, 5.0f, 0.0f));
        this->_velocity.y = 4.0f;
        jumpCount += 1;
    }
    else if (jumpCount == 0 && this->_velocity.y <= 0)
    {
        walking = 0;
        recentlyMoved = 0;
        //this->AddVelocity(glm::vec3(0.0f, 5.0f, 0.0f));
        this->_velocity.y = 4.0f;
        jumpCount += 1;
    }
}

void PlayerJumper::onCollision(Object3D *other) {
    if((other->tag == "floor" || other->tag == "platform" || other->tag == "moving platform") && _velocity.y != 0)
    {

        this->switchAnimationWalk();
        this->switchAnimationStand();


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

void PlayerJumper::switchAnimationWalk() {
    if(walking == 0 && (_velocity.x != 0 || _velocity.z != 0) ){
//        this->loadAnimation("res/models/Players/Mich3l/michel_running.dae");
//Animator tempA(&walkA);
//this->animator= tempA;
animator.PlayAnimation(&walkA);
        this->recentlyMoved = 0;
        this->walking = 1;
    }
}

void PlayerJumper::switchAnimationJump() {
    if(jumpCount == 0){
//        this->loadAnimation("res/models/Players/Mich3l/michel_jumping.dae");
//        Animator tempA(&jumpA);
//        this->animator= tempA;
        this->animator.PlayAnimation(&jumpA);
    }

}

void PlayerJumper::switchAnimationStand() {
    if(_velocity.x == 0 && _velocity.z == 0 && recentlyMoved == 0) {
//        this->loadAnimation("res/models/Players/Mich3l/michel_breathing_and_looking_around.dae");
        Animator tempA(&stamdA);
        this->animator= tempA;
        this->recentlyMoved = 1;
        this->walking = 0;
    }
}


void PlayerJumper::loadAnimations() {
    Animation temp("res/models/Players/Mich3l/michel_running.dae", &*this->_model);
    Animation temp2("res/models/Players/Mich3l/michel_jumping.dae", &*this->_model);
    Animation temp3("res/models/Players/Mich3l/michel_breathing_and_looking_around.dae", &*this->_model);
    walkA = temp;
    stamdA = temp3;
    jumpA = temp2;
    this->animator.PlayAnimation(&temp);

}



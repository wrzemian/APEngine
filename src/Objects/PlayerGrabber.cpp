//
// Created by Karol on 25.05.2023.
//

#include "../../include/Objects/PlayerGrabber.h"

void PlayerGrabber::initPlayer(InputSystem* inputSystem) {
    loadFromJSON(Engine::parser.CreateFromJSONMovingObject("objects/movingObj_1"));
    tag = "player";
    type = "grabber";
    inputSystem->monitorKey(GLFW_KEY_UP);
    inputSystem->monitorKey(GLFW_KEY_LEFT);
    inputSystem->monitorKey(GLFW_KEY_RIGHT);
    inputSystem->monitorKey(GLFW_KEY_DOWN);
    inputSystem->monitorKey(GLFW_KEY_KP_2);
    inputSystem->monitorKey(GLFW_KEY_KP_3);
    //inputSystem->monitorKey(GLFW_KEY_SPACE);
    this->loadAnimations();
}


void PlayerGrabber::UpdatePlayer(InputSystem* inputSystem, float movementSpeed) {
    if (inputSystem->GetKey(GLFW_KEY_UP)) {
        direction.z = -1.0f;
    }
    else if (inputSystem->GetKey(GLFW_KEY_DOWN)) {
        direction.z = 1.0f;
    }
    else if(inputSystem->getJoystickAxis(0, GLFW_GAMEPAD_AXIS_LEFT_Y) >= padJoystickTolerance || inputSystem->getJoystickAxis(0, GLFW_GAMEPAD_AXIS_LEFT_Y) <= -padJoystickTolerance)
    {
        direction.z = inputSystem->getJoystickAxis(0, GLFW_GAMEPAD_AXIS_LEFT_Y);
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
    else if(inputSystem->getJoystickAxis(0, GLFW_GAMEPAD_AXIS_LEFT_X) >= padJoystickTolerance || inputSystem->getJoystickAxis(0, GLFW_GAMEPAD_AXIS_LEFT_X) <= -padJoystickTolerance)
    {
        direction.x = inputSystem->getJoystickAxis(0, GLFW_GAMEPAD_AXIS_LEFT_X);
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

    if(_velocity.x!=0 || _velocity.z!=0) {
        _transform._rotation.y = -atan2(_velocity.z, _velocity.x);
    }

    if (inputSystem->GetKeyDown(GLFW_KEY_KP_2) || inputSystem->GetGamepadButtonDown(0, GLFW_GAMEPAD_BUTTON_X)) {
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
    if (inputSystem->GetKeyDown(GLFW_KEY_KP_3) || inputSystem->GetGamepadButtonDown(0, GLFW_GAMEPAD_BUTTON_Y)) {
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
    if(other->tag == "battery")
    {
        canPickUpBattery = true;
    }
}

void PlayerGrabber::Jump() {
    if (jumpCount == 0 && this->_velocity.y <= 0&& this->_velocity.y <= 0)
    {
        this->switchAnimationJump();
        walking = 0;
        recentlyMoved = 0;
        this->switchAnimationJump();
        this->AddVelocity(glm::vec3(0.0f, 4.0f, 0.0f));
        jumpCount += 1;
    }
}

void PlayerGrabber::Grab() {
    if (haveBattery)
    {
        grabber->Grab();
        this->switchAnimationGrab();
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
    _transform._scale.x = 0.02f;
    _transform._scale.y = 0.02f;
    _transform._scale.z = 0.02f;
}

void PlayerGrabber::switchAnimationWalk() {
    if(walking == 0 && (_velocity.x != 0 || _velocity.z != 0) ){
        this->animator.PlayAnimation(&walkP);
        this->grabber->animator.PlayAnimation(&this->grabber->walkA);
//        this->loadAnimation("res/models/Players/Cr4nk/REST_CRANK_WALKING.dae");
//        this->grabber->loadAnimation("res/models/Players/Cr4nk/RIGHT_HAND_CRANK_WALKING.dae");
        this->recentlyMoved = 0;
        this->walking = 1;
    }
}

void PlayerGrabber::switchAnimationJump() {
    if(jumpCount == 0){
        this->animator.PlayAnimation(&jumpP);
        this->grabber->animator.PlayAnimation(&this->grabber->jumpA);
    }
}

void PlayerGrabber::switchAnimationStand() {
    if (_velocity.x == 0 && _velocity.z == 0 && recentlyMoved == 0) {
//        this->loadAnimation("res/models/Players/Cr4nk/REST_CRANK_STANDING.dae");
//        this->grabber->loadAnimation("res/models/Players/Cr4nk/RIGHT_HAND_CRANK_STANDING.dae");
        this->animator.PlayAnimation(&standP);
        this->grabber->animator.PlayAnimation(&this->grabber->stamdA);
        this->recentlyMoved = 1;
        this->walking = 0;
    }
}

void PlayerGrabber::switchAnimationGrab() {
//        this->loadAnimation("res/models/Players/Cr4nk/REST_CRANK_HOOKING.dae");
//    this->grabber->loadAnimation("res/models/Players/Cr4nk/RIGHT_HAND_CRANK_HOOKING.dae");
    this->animator.PlayAnimation(&hookingP);
    this->grabber->animator.PlayAnimation(&this->grabber->hookingA);
}

void PlayerGrabber::onCollisionY(Object3D *other) {
    MovingObject::onCollisionY(other);
    if((other->tag == "floor" || other->tag == "platform" || other->tag == "moving platform" || other->tag == "box"))
    {
        this->switchAnimationWalk();
        this->switchAnimationStand();
        _velocity.y = 0;
        jumpCount = 0;

    }

}

void PlayerGrabber::unusualCollision(Object3D *other) {
    Object3D::unusualCollision(other);
    _velocity.y = 0;
    jumpCount = 0;
}

void PlayerGrabber::loadAnimations() {
    //body animations
    Animation temp5("res/models/Players/Cr4nk/REST_CRANK_WALKING.dae", &*this->_model);
    Animation temp6("res/models/Players/Cr4nk/REST_CRANK_JUMPING.dae", &*this->_model);
    Animation temp7("res/models/Players/Cr4nk/REST_CRANK_STANDING.dae", &*this->_model);
    Animation temp8("res/models/Players/Cr4nk/REST_CRANK_HOOKING.dae", &*this->_model);

    walkP = temp5;
    standP = temp7;
    jumpP = temp6;
    hookingP = temp8;



    this->animator.PlayAnimation(&temp5);
}


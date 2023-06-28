//
// Created by Karol on 23.05.2023.
//

#include "../../include/Objects/PlayerJumper.h"
#include "../../include/Audio/AudioManager.h"

//Animation walkA, stamdA, jumpA;

void PlayerJumper::initPlayer(InputSystem* inputSystem) {
    loadFromJSON(Engine::parser.CreateFromJSONMovingObject("objects/movingObj_0"));
    tag = "player";
    type = "jumper";
    inputSystem->monitorKey(GLFW_KEY_W);
    inputSystem->monitorKey(GLFW_KEY_A);
    inputSystem->monitorKey(GLFW_KEY_S);
    inputSystem->monitorKey(GLFW_KEY_D);
    inputSystem->monitorKey(GLFW_KEY_E);
    inputSystem->monitorKey(GLFW_KEY_T);
    //inputSystem->monitorKey(GLFW_KEY_SPACE);

    loadAnimations();
    currentAnimation = stamdA;
    previousAnimation = walkA;
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
        DropBattery();
        if (canPickUpBattery)
        {
            haveBattery = true;
            canPickUpBattery= false;
            std::cout<< "pick up battery"<<std::endl;
        }
    }
    if (inputSystem->GetKeyDown(GLFW_KEY_T) || inputSystem->GetGamepadButtonDown(1, GLFW_GAMEPAD_BUTTON_B)) {
        if(!pickedUpBox && lastTouchedBox != nullptr && lastTouchedBox->canBePickedUp)
        {
            pickedUpBox = true;
            lastTouchedBox->SwitchGravity(false);
            lastTouchedBox->canBePickedUp = false;
        }
        else if(pickedUpBox && lastTouchedBox != nullptr && !lastTouchedBox->canBePickedUp)
        {
            pickedUpBox = false;
            lastTouchedBox->SwitchGravity(true);
            lastTouchedBox->canBePickedUp = true;
        }
    }
   /* if (inputSystem->GetKeyDown(GLFW_KEY_SPACE)) {
        Jump();
        std::cout<<_velocity.y<<std::endl;
        std::cout<<"help"<<std::endl;
    }*/
   if(haveBattery)
   {
       battery->_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
       glm::quat playerQuat = glm::quat(_transform._rotation); // Convert Euler angles to quaternion
       glm::mat4 rotationMat = glm::mat4_cast(playerQuat); // Convert quaternion to rotation matrix
       battery->_transform._position = _transform._position + glm::vec3(rotationMat * glm::vec4(batteryOffset, 1.0f) )- battery->modelMiddle;
   }
   if(pickedUpBox)
   {
        glm::quat playerQuat = glm::quat(_transform._rotation); // Convert Euler angles to quaternion
        glm::mat4 rotationMat = glm::mat4_cast(playerQuat); // Convert quaternion to rotation matrix
        lastTouchedBox->_transform._position = _transform._position + glm::vec3(rotationMat * glm::vec4(boxOffset, 1.0f) )  - lastTouchedBox->modelMiddle;
   }
}

void PlayerJumper::Jump() {
    if (jumpCount == 1 && haveBattery)
    {
        this->switchAnimationJump();
        AudioManager::GetInstance()->PauseSound(Audio::MICHEL_JUMP);
        AudioManager::GetInstance()->PlaySound(Audio::JETPACK);
        walking = 0;
        recentlyMoved = 0;
        //this->AddVelocity(glm::vec3(0.0f, 5.0f, 0.0f));
        this->_velocity.y = 4.0f;
        jumpCount += 1;
        recentlyJumped = 0;
    }
    else if (jumpCount == 0 && this->_velocity.y <= 0)
    {
        walking = 0;
        recentlyMoved = 0;
        this->switchAnimationJump();
        //this->AddVelocity(glm::vec3(0.0f, 5.0f, 0.0f));
        AudioManager::GetInstance()->PlaySound(Audio::MICHEL_JUMP);
        this->_velocity.y = 4.0f;
        jumpCount += 1;
        recentlyJumped = 0;
    }
}

void PlayerJumper::onCollision(Object3D *other) {
   /* if((other->tag == "floor" || other->tag == "platform" || other->tag == "moving platform") && _velocity.y != 0)
    {

        this->switchAnimationWalk();
        this->switchAnimationStand();


        _velocity.y = 0;
        jumpCount = 0;
    }*/
    if(other->tag == "battery")
    {
        canPickUpBattery = true;
    }
    if(other->tag == "box")
    {
        lastTouchedBox = other;
    }
}

void PlayerJumper::onCollisionExit(Object3D *other) {
    Object3D::onCollisionExit(other);
    if(other->tag == "battery")
    {
        canPickUpBattery = false;
    }
    if(other->tag == "box")
    {
        if(other == lastTouchedBox && !pickedUpBox)
        {
            lastTouchedBox = nullptr;
        }
    }
}

void PlayerJumper::onCollisionY(Object3D *other) {
    MovingObject::onCollisionY(other);
    if(other->tag == "floor" || other->tag == "platform" || other->tag == "moving platform" )
    {
        if(recentlyJumped == 0){
            AudioManager::GetInstance()->PauseSound(Audio::MICHEL_JUMP);
            AudioManager::GetInstance()->PlaySound(Audio::MICHEL_WALK);
            recentlyJumped = 1;
        }
        this->switchAnimationWalk();
        this->switchAnimationStand();
        _velocity.y = 0;
        jumpCount = 0;
    }
    if(other->tag == "box")
    {
        jumpCount = 0;
        this->switchAnimationWalk();
        this->switchAnimationStand();
        std::cout << "jd z gory gracz" << std::endl;
    }
}

void PlayerJumper::unusualCollision(Object3D *other) {
    Object3D::unusualCollision(other);
    _velocity.y = 0;
    this->switchAnimationWalk();
    this->switchAnimationStand();
    jumpCount = 0;
}

//void PlayerJumper::switchAnimationWalk() {
//    if(walking == 0 && (_velocity.x != 0 || _velocity.z != 0) ){
//        this->loadAnimation("res/models/Players/Mich3l/michel_running.dae");
//Animator tempA(&walkA);
//this->animator= tempA;
void PlayerJumper::switchAnimationWalk() {
    if(walking == 0 && (_velocity.x != 0 || _velocity.z != 0) ){
        currentAnimation = walkA;
        previousAnimation = stamdA;
        animationTimer = 0;
        animator.PlayAnimation(&walkA);
        AudioManager::GetInstance()->PlaySound(Audio::MICHEL_LAND);
        this->recentlyMoved = 0;
        this->walking = 1;
    }
}

void PlayerJumper::switchAnimationJump() {
    if(jumpCount == 0){
//        this->loadAnimation("res/models/Players/Mich3l/michel_jumping.dae");
//        Animator tempA(&jumpA);
//        this->animator= tempA;
        AudioManager::GetInstance()->PauseSound(Audio::MICHEL_LAND);
        currentAnimation = jumpA;
        if(this->walking == 1) {
            previousAnimation = walkA;
        } else {
            previousAnimation = stamdA;
        }
        animationTimer = 0;
        this->animator.PlayAnimation(&jumpA);

    }

}

void PlayerJumper::switchAnimationStand() {
    if(_velocity.x == 0 && _velocity.z == 0 && recentlyMoved == 0) {
        AudioManager::GetInstance()->PauseSound(Audio::MICHEL_LAND);
//        this->loadAnimation("res/models/Players/Mich3l/michel_breathing_and_looking_around.dae");
//        Animator tempA(&stamdA);
//        this->animator= tempA;
        currentAnimation = stamdA;
        if(this->walking == 1) {
            previousAnimation = walkA;
        } else {
            previousAnimation = stamdA;
        }
        animationTimer = 0;
        this->animator.PlayAnimation(&stamdA);
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

void PlayerJumper::DropBattery() {
    if(haveBattery)
    {
        std::cout<< "drop the battery"<<std::endl;
        haveBattery = false;
        canPickUpBattery= false;
    }
}



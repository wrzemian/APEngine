#include "../../include/Objects/Ant.h"
#include "../../include/Engine.h"
#include "random.hpp"
using Random = effolkronium::random_static;

Ant::Ant() {
    //_windowName = fmt::format("Moving object #{}", Engine::getImguiIndex());
    IGui::setWindowName("ant");
    //spdlog::warn("moving object constructor");
    timer = 0;
    minSpeed = Random::get(3.0f, 0.5f);
    maxSpeed = maxSpeed + Random::get(2.0f, 0.5f);
    minMoveTime = Random::get(1, 3);
    maxMoveTime = minMoveTime + Random::get(1, 2);
    refreshTime = Random::get(1, 3);
    movingTime = 0;
    isMoving = false;
    Engine::addAnt(this);
}



Ant::~Ant() {
    Engine::removeAnt(this);
}

void Ant::Move(float deltaTime) {
//    spdlog::warn("refreshTime {}, timer {}", refreshTime, timer);
    if(!isMoving) {
        timer += deltaTime;
    }

    if(timer >= refreshTime) {
        timer = 0;
        isMoving = true;
        movingTime = Random::get(minMoveTime, maxMoveTime);
        float angle = Random::get(0.0f, 2 * glm::pi<float>());
        _velocity.x = cos(angle) * Random::get(minSpeed, maxSpeed);
        _velocity.z = sin(angle) * Random::get(minSpeed, maxSpeed);
        //spdlog::warn("starting movement for {} in X {}, Z {}", movingTime, _velocity.x, _velocity.z);

    }
    if(isMoving && movingTime < 0) {
       // spdlog::warn("stopping movement");
        isMoving = false;
        movingTime = 0;
        _velocity.x = 0;
        _velocity.z = 0;
    }
    if (isMoving) {
        movingTime -= deltaTime;
    }
    MovingObject::Move(deltaTime);
}

void Ant::Escape(std::shared_ptr<Object3D> other) {
    glm::vec2 AntPlayerVec(other->_transform._position.x - this->_transform._position.x,
                           other->_transform._position.z - this->_transform._position.z);
    glm::normalize(AntPlayerVec);
    isMoving = true;
    movingTime =minMoveTime;
    _velocity.x = -AntPlayerVec.x * maxSpeed;
    _velocity.z = -AntPlayerVec.y * minSpeed;
}

void Ant::onCollisionX(std::shared_ptr<Object3D> other) {
    if(other->tag == "floor") {
        //spdlog::info("ant hit X wall");
        _velocity.x = 0;
        _velocity.z = 0;
    }
    if(other->tag == "player") {
        //spdlog::info("ant player X");
        this->Escape(other);
    }
}

void Ant::onCollisionY(std::shared_ptr<Object3D> other) {
    if(other->tag == "floor") {
        _velocity.y = 0;
    }
}

void Ant::onCollisionZ(std::shared_ptr<Object3D> other) {
    if(other->tag == "floor") {
        //spdlog::info("ant hit Z wall");
        _velocity.x = 0;
        _velocity.z = 0;
    }
    if(other->tag == "player") {
        //spdlog::info("ant player Z");
        this->Escape(other);
    }
}

void Ant::ImGui() {
    super::ImGui();

    ImGui::Begin(getWindowName().c_str());
    ImGui::End();
}
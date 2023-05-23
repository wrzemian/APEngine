#include "../../include/Objects/Ant.h"
#include "../../include/Engine.h"
#include "random.hpp"
using Random = effolkronium::random_static;

Ant::Ant() {
    //windowName = fmt::format("Moving object #{}", Engine::getImguiIndex());
    IGui::setWindowName("ant");
    //spdlog::warn("moving object constructor");
    timer = 0;
    minSpeed = Random::get(3.0f, 0.5f);
    maxSpeed = maxSpeed + Random::get(2.0f, 0.5f);
    minMoveTime = Random::get(1, 3);
    maxMoveTime = minMoveTime + Random::get(1, 2);
    refreshTime = Random::get(3, 30);
    movingTime = 0;
    isMoving = false;
    Engine::addAnt(this);
}



Ant::~Ant() {
    Engine::removeAnt(this);
}

void Ant::Move(float deltaTime) {
//    spdlog::warn("refreshTime {}, timer {}", refreshTime, timer);
    if(timer >= refreshTime) {
        timer = 0;
        isMoving = true;
        movingTime = Random::get(minMoveTime, maxMoveTime);
        spdlog::warn("starting movement for {}", movingTime);
        const float PI = 3.14159265358979323846f;
        float angle = Random::get(0.0f, 2 * PI);
        _velocity.x = cos(angle) * Random::get(minSpeed, maxSpeed);
        _velocity.z = sin(angle) * Random::get(minSpeed, maxSpeed);
    }
    if(isMoving && movingTime < 0) {
        spdlog::warn("stopping movement");
        isMoving = false;
        movingTime = 0;
        _velocity.x = 0;
        _velocity.z = 0;
    }
    if (isMoving) {
        _transform._position += _velocity * deltaTime;
        movingTime -= deltaTime;
    }
}

void Ant::onCollision(Object3D *other) {
//    spdlog::warn("ant colliding");
//    _velocity.x = 0;
//    _velocity.y = 0;
//    _velocity.z = 0;
}

void Ant::ImGui() {
    super::ImGui();

    ImGui::Begin(windowName.c_str());
    ImGui::End();
}
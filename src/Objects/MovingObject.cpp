//
// Created by micha on 02.04.2023.
//

#include "../../include/Engine.h"
#include "../include/Objects/MovingObject.h"
#include "../imgui_impl/imgui_impl_glfw.h"
#include "../imgui_impl/imgui_impl_opengl3.h"

MovingObject::MovingObject() {
    //windowName = fmt::format("Moving object #{}", Engine::getImguiIndex());
    IGui::setWindowName("moving object");
    spdlog::info("moving object constructor");

    Engine::addMovingObject(this);
}



MovingObject::~MovingObject() {
    spdlog::error("moving object destructor");
    Engine::removeMovingObject(this);
}

void MovingObject::SetVelocity(glm::vec3 velocity) {
    _velocity = velocity;
}

void MovingObject::AddVelocity(glm::vec3 velocity) {
    _velocity += velocity;
}

void MovingObject::StopMoving() {
    _velocity.x = 0;
    _velocity.y = 0;
    _velocity.z = 0;
}

void MovingObject::ImGui() {
    super::ImGui();

    ImGui::Begin(windowName.c_str());
    ImGui::SetWindowSize(ImVec2(300, 440));

    ImGui::SliderFloat("velocity X", &_velocity.x, -10.0f, 10.0f);
    ImGui::SliderFloat("velocity Y", &_velocity.y, -10.0f, 10.0f);
    ImGui::SliderFloat("velocity Z", &_velocity.z, -10.0f, 10.0f);

    if (ImGui::Button("Stop")) {
        this->StopMoving();
    }

    if (ImGui::Button("SAVE MOV OBJ")) {

        Engine::parser.SaveJSON(this->ParseToJSON(), "objects/movingObj_" + std::to_string(Engine::getMovingObjectIndex(this)));
    }
    ImGui::End();
}

void MovingObject::Move(float deltaTime) {
    _transform._position += _velocity * deltaTime;
    _velocity += _gravity * deltaTime;

//    pPosition += pVelocity * deltaTime + real_gravity * deltaTime * deltaTime / 2.0;
//    pVelocity += real_gravity * deltaTime;
}

void MovingObject::onCollision(Object3D *other) {
//    _velocity.x = 0;
    _velocity.y = 0;
//    _velocity.z = 0;
}

rapidjson::Document MovingObject::ParseToJSON() {
    rapidjson::Document d;
    d.SetObject();
    d.AddMember("type", "object3D", d.GetAllocator());
    d.AddMember("scaleX", _transform._scale.x, d.GetAllocator());
    d.AddMember("scaleY", _transform._scale.y, d.GetAllocator());
    d.AddMember("scaleZ", _transform._scale.z, d.GetAllocator());
    d.AddMember("rotationX", _transform._rotation.x, d.GetAllocator());
    d.AddMember("rotationY", _transform._rotation.y, d.GetAllocator());
    d.AddMember("rotationZ", _transform._rotation.z, d.GetAllocator());
    d.AddMember("positionX", _transform._position.x, d.GetAllocator());
    d.AddMember("positionY", _transform._position.y, d.GetAllocator());
    d.AddMember("positionZ", _transform._position.z, d.GetAllocator());
    d.AddMember("velocityX", _velocity.x, d.GetAllocator());
    d.AddMember("velocityY", _velocity.y, d.GetAllocator());
    d.AddMember("velocityZ", _velocity.z, d.GetAllocator());
    rapidjson::Value n(_path.c_str(), d.GetAllocator());
    d.AddMember("model", n, d.GetAllocator());


    return d;
}

void MovingObject::loadFromJSON(MovingObject temp) {
    this->loadModel(temp._path);
    this->_transform._scale = temp._transform._scale;
    this->_transform._rotation = temp._transform._rotation;
    this->_transform._position = temp._transform._position;
    this->_velocity = temp._velocity;
}





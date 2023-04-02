//
// Created by micha on 02.04.2023.
//

#include "../include/Engine.h"
#include "../include/MovingObject.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"

MovingObject::MovingObject() {}

MovingObject::~MovingObject() {

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

    ImGui::Begin("MovingObject");
    ImGui::SetWindowSize(ImVec2(300, 120));

    ImGui::SliderFloat("velocity X", &_velocity.x, -10.0f, 10.0f);
    ImGui::SliderFloat("velocity Y", &_velocity.y, -10.0f, 10.0f);
    ImGui::SliderFloat("velocity Z", &_velocity.z, -10.0f, 10.0f);

    if (ImGui::Button("Stop")) {
        this->StopMoving();
    }

    ImGui::End();
}

void MovingObject::Move() {
    _transform._position += _velocity * Engine::deltaTime;
}


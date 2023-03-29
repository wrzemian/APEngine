//
// Created by micha on 30.03.2023.
//

#include "../include/Object3D.h"
#include "../include/Transform.h"

#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"

Object3D::Object3D() {
    _transform = Transform();
}

Object3D::~Object3D() {

}

void Object3D::Draw(Shader &shader) {
    _transform.updateWorldTransform(glm::mat4(1.0f), shader);
    // shader.setMat4("model", _transform.getModel());
    _model.Draw(shader);

}

void Object3D::loadModel(const std::string &path) {
    Model temp(path);
    _model = temp;
}

void Object3D::ImGui() {
    ImGui::Begin("3D Object");
    ImGui::SetWindowSize(ImVec2(250, 300));

    ImGui::SliderFloat("scale X", &_transform._scale.x, -10.0f, 10.0f);
    ImGui::SliderFloat("scale Y", &_transform._scale.y, -10.0f, 10.0f);
    ImGui::SliderFloat("scale Z", &_transform._scale.z, -10.0f, 10.0f);

    ImGui::SliderFloat("rotation X", &_transform._rotation.x, -6.5, 6.5f);
    ImGui::SliderFloat("rotation Y", &_transform._rotation.y, -6.5, 6.5f);
    ImGui::SliderFloat("rotation Z", &_transform._rotation.z, -6.5, 6.5f);

    ImGui::SliderFloat("position X", &_transform._position.x, -20.0f, 20.0f);
    ImGui::SliderFloat("position Y", &_transform._position.y, -20.0f, 20.0f);
    ImGui::SliderFloat("position Z", &_transform._position.z, -20.0f, 20.0f);


    ImGui::End();
}

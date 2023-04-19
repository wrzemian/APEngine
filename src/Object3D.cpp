//
// Created by micha on 30.03.2023.
//

#include "../include/Object3D.h"
#include "../include/Transform.h"
#include "../include/Engine.h"

#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"

Object3D::Object3D() {
    _transform = Transform();
    loadedModel = false;
    Engine::addObject(this);
//    windowName = fmt::format("Object3D #{}", Engine::getImguiIndex());
//    spdlog::warn("Object3d window name = {}", windowName);
//
//    Engine::addImgui(this);
}

Object3D::~Object3D() = default;

void Object3D::Draw() {
    if(loadedModel == false) {
        return;
    }
    _shader->use();
    std::cout<<"shader: " <<_shader <<" _ " << &_shader;
    _transform.updateWorldTransform(glm::mat4(1.0f), *_shader);
    // shader.setMat4("model", _transform.getModel());
    _model.Draw(*_shader);

}

void Object3D::loadModel(const std::string &path) {
    Model temp(path);
    _model = temp;
    loadedModel = true;
}

void Object3D::ImGui() {
    ImGui::Begin(windowName.c_str());

    ImGui::SetWindowSize(ImVec2(300, 310));

    ImGui::SliderFloat("scale X", &_transform._scale.x, 0.0f, 2.0f);
    ImGui::SliderFloat("scale Y", &_transform._scale.y, 0.0f, 2.0f);
    ImGui::SliderFloat("scale Z", &_transform._scale.z, 0.0f, 2.0f);

    ImGui::SliderFloat("rotation X", &_transform._rotation.x, -6.5, 6.5f);
    ImGui::SliderFloat("rotation Y", &_transform._rotation.y, -6.5, 6.5f);
    ImGui::SliderFloat("rotation Z", &_transform._rotation.z, -6.5, 6.5f);

    ImGui::SliderFloat("position X", &_transform._position.x, -20.0f, 20.0f);
    ImGui::SliderFloat("position Y", &_transform._position.y, -20.0f, 20.0f);
    ImGui::SliderFloat("position Z", &_transform._position.z, -20.0f, 20.0f);

    if (ImGui::Button("Reset Scale")) {
        _transform._scale = glm::vec3(1,1,1);
    }

    if (ImGui::Button("Reset Position")) {
        _transform._position = glm::vec3(0,0,0);
    }
    if (ImGui::Button("Reset Rotation")) {
        _transform._rotation = glm::vec3(0,0,0);
    }

    ImGui::End();
}

void Object3D::onCollision(Object3D *other) {
    spdlog::warn("Object3D empty onCollision");
}

void Object3D::setShader(Shader *shader) {
    _shader = shader;
}

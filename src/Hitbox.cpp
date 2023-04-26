//
// Created by micha on 02.04.2023.
//

#include "../include/Hitbox.h"

#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"
#include "../include/Engine.h"

void Hitbox::Create(Object3D* object, glm::vec3 offset) {
    _object = object;
    _position = &object->_transform._position;
    _offset = offset;
    _color = glm::vec3(1,1,0);

    debugShape.Initialize();
}

void Hitbox::Draw(glm::mat4 projectionView) {
    if(!draw) {
        return;
    }
    //spdlog::info("drawing hitbox {}", windowName);

    debugShape.shader.use();
    debugShape.shader.setVec3("offset", _offset);
    debugShape.shader.setVec3("color", _color);

    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, *_position);
    //model = glm::scale(model, _dimensions);

    //glm::mat4 model = glm::scale(_transform->_localTransform, _dimensions);

    debugShape.shader.setMat4("model", model);
    debugShape.shader.setMat4("projectionView", projectionView);

    debugShape.DrawCube(_min, _max);
}

void Hitbox::ImGui()  {
    ImGui::Begin(windowName.c_str());
    ImGui::SetWindowSize(ImVec2(300, 380));

    ImGui::SliderFloat("min X", &_min.x, -5.0f, 5.0f);
    ImGui::SliderFloat("min Y", &_min.y, -5.0f, 5.0f);
    ImGui::SliderFloat("min Z", &_min.z, -5.0f, 5.0f);

    ImGui::SliderFloat("max X", &_max.x, -5.0f, 5.0f);
    ImGui::SliderFloat("max Y", &_max.y, -5.0f, 5.0f);
    ImGui::SliderFloat("max Z", &_max.z, -5.0f, 5.0f);

    ImGui::SliderFloat("offset X", &_offset.x, -5, 5);
    ImGui::SliderFloat("offset Y", &_offset.y, -5, 5);
    ImGui::SliderFloat("offset Z", &_offset.z, -5, 5);

    ImGui::SliderFloat("R", &_color.x, 0, 1);
    ImGui::SliderFloat("G", &_color.y, 0, 1);
    ImGui::SliderFloat("B", &_color.z, 0, 1);

    if (ImGui::Button("Reset Offset")) {
        _offset = glm::vec3(0,0,0);
    }
    ImGui::Checkbox("Draw: ", &draw);
    ImGui::End();

}

bool Hitbox::TestForIntersection(Hitbox &other) {
    //spdlog::info("1st hitbox: ({}, {}) ({}, {})");
    bool intersects = _position->x + _offset.x + _min.x <= other._position->x + other._offset.x + other._max.x &&
                      _position->x + _offset.x + _max.x >= other._position->x + other._offset.x + other._min.x &&
                      _position->y + _offset.y + _min.y <= other._position->y + other._offset.y + other._max.y &&
                      _position->y + _offset.y + _max.y >= other._position->y + other._offset.y + other._min.y &&
                      _position->z + _offset.z + _min.z <= other._position->z + other._offset.z + other._max.z &&
                      _position->z + _offset.z + _max.z >= other._position->z + other._offset.z + other._min.z;
    if(intersects) {
        _object->onCollision(other._object);

        _color.x = 1;
        _color.y = 0;
    } else {
        _color.x = 0;
        _color.y = 1;
    }


    return intersects;
}

Hitbox::Hitbox() {
    _min = glm::vec3(-1, -1, -1);
    _max = glm::vec3(1, 1, 1);

    IGui::setWindowName("hitbox");
    Engine::addHitbox(this);
    //Engine::addImgui(this);
}

Hitbox::~Hitbox() {
    Engine::removeHitbox(this);
}






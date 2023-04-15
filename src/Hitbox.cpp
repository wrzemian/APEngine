//
// Created by micha on 02.04.2023.
//

#include "../include/Hitbox.h"

#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"

void Hitbox::Create(Transform* transform, glm::vec3 dimensions, glm::vec3 offset) {
    _transform = transform;
    _position = &transform->_position;
    _dimensions = dimensions;
    _min = glm::vec3(-1, -1, -1);
    _max = glm::vec3(1, 1, 1);
    _offset = offset;

    debugShape.Initialize();
}

void Hitbox::Draw(glm::mat4 projectionView) {
    if(!draw) {
        return;
    }
    debugShape.shader.use();
    debugShape.shader.setVec3("offset", _offset);

    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, *_position);
    //model = glm::scale(model, _dimensions);

    //glm::mat4 model = glm::scale(_transform->_localTransform, _dimensions);

    debugShape.shader.setMat4("model", model);
    debugShape.shader.setMat4("projectionView", projectionView);

    debugShape.DrawCube(_min, _max);
}

void Hitbox::ImGui() {
    ImGui::Begin("Hitbox");
    ImGui::SetWindowSize(ImVec2(300, 280));

    ImGui::SliderFloat("min X", &_min.x, -5.0f, 5.0f);
    ImGui::SliderFloat("min Y", &_min.y, -5.0f, 5.0f);
    ImGui::SliderFloat("min Z", &_min.z, -5.0f, 5.0f);

    ImGui::SliderFloat("max X", &_max.x, -5.0f, 5.0f);
    ImGui::SliderFloat("max Y", &_max.y, -5.0f, 5.0f);
    ImGui::SliderFloat("max Z", &_max.z, -5.0f, 5.0f);

    ImGui::SliderFloat("offset X", &_offset.x, -5, 5);
    ImGui::SliderFloat("offset Y", &_offset.y, -5, 5);
    ImGui::SliderFloat("offset Z", &_offset.z, -5, 5);

    if (ImGui::Button("Reset Offset")) {
        _offset = glm::vec3(0,0,0);
    }
    ImGui::Checkbox("Draw: ", &draw);
    ImGui::End();

}




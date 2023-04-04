//
// Created by micha on 02.04.2023.
//

#include "../include/Hitbox.h"

#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"

void Hitbox::Create(Transform* transform, glm::vec3 dimensions, glm::vec3 offset) {
    _transform = transform;
    _dimensions = dimensions;
    _offset = offset;

    debugShape.Initialize();
}

void Hitbox::Draw(glm::mat4 projectionView) {
    if(!draw) {
        return;
    }
    debugShape.shader.use();
    debugShape.shader.setVec3("offset", _offset);
    glm::mat4 model = glm::scale(_transform->_localTransform, _dimensions);

    debugShape.shader.setMat4("model", model);
    debugShape.shader.setMat4("projectionView", projectionView);

    debugShape.DrawCube();
}

void Hitbox::ImGui() {
    ImGui::Begin("Hitbox");
    ImGui::SetWindowSize(ImVec2(300, 210));

    ImGui::SliderFloat("dimension X", &_dimensions.x, 0.0f, 5.0f);
    ImGui::SliderFloat("dimension Y", &_dimensions.y, 0.0f, 5.0f);
    ImGui::SliderFloat("dimension Z", &_dimensions.z, 0.0f, 5.0f);

    ImGui::SliderFloat("offset X", &_offset.x, -5, 5);
    ImGui::SliderFloat("offset Y", &_offset.y, -5, 5);
    ImGui::SliderFloat("offset Z", &_offset.z, -5, 5);

    if (ImGui::Button("Reset Offset")) {
        _offset = glm::vec3(0,0,0);
    }
    ImGui::Checkbox("Draw: ", &draw);
    ImGui::End();

}




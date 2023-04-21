//
// Created by micha on 21.04.2023.
//

#include "../include/Walls.h"
#include "spdlog/spdlog.h"


void Walls::calculateHitboxes() {
    for(Mesh mesh: _model.meshes) {
        float minX = mesh.vertices.at(0).Position.x;
        float maxX = mesh.vertices.at(0).Position.x;

        float minY = mesh.vertices.at(0).Position.y;
        float maxY = mesh.vertices.at(0).Position.y;

        for(auto vertex: mesh.vertices) {
            if(vertex.Position.x > maxX) {
                maxX = vertex.Position.x;
            }
            if(vertex.Position.x < minX) {
                minX = vertex.Position.x;
            }

            if(vertex.Position.y > maxY) {
                maxY = vertex.Position.y;
            }
            if(vertex.Position.y < minY) {
                minY = vertex.Position.y;
            }
        }

        //std::unique_ptr<Hitbox> hitbox = std::make_unique<Hitbox>();
//        Hitbox hitbox; //TODO: create hitbox and save it to list
//        //Hitbox* hitbox = new Hitbox();
//        hitbox._min.x = minX;
//        hitbox._max.x = maxX;
//        hitbox._min.y = minY;
//        hitbox._max.y = maxY;

        spdlog::info("Hitbox from ({}, {}) to ({}, {})", minX, minY, maxX, maxY);

        //hitboxes.push_back(hitbox);
    }
}

void Walls::ImGui() {
    ImGui::Begin(windowName.c_str());
    ImGui::SetWindowSize(ImVec2(300, 500));
    Object3D::ImGui();

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

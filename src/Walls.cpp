//
// Created by micha on 21.04.2023.
//

#include "../include/Walls.h"
#include "../include/Hitbox.h"

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

//        std::unique_ptr<Hitbox> hitbox = std::make_unique<Hitbox>();
 //      Hitbox hitbox; //TODO: create hitbox and save it to list
        Hitbox* hitbox = new Hitbox();
        hitbox->Create(this);
        hitbox->_min.x = minX;
        hitbox->_max.x = maxX;
        hitbox->_min.y = minY;
        hitbox->_max.y = maxY;

        spdlog::info("Hitbox from ({}, {}) to ({}, {})", minX, minY, maxX, maxY);

        hitboxes.push_back(hitbox);
    }
}

void Walls::ImGui() {
    ImGui::Begin(windowName.c_str());
    Object3D::ImGui();

    ImGui::SetWindowSize(ImVec2(300, 600));

    if (ImGui::Button("Show hitboxes")) {
        spdlog::warn("showing hitboxes");
        for(Hitbox* hitbox: hitboxes) {
            hitbox->draw = true;
        }
    }

    if (ImGui::Button("Hide hitboxes")) {
        spdlog::warn("hiding hitboxes");
        for(Hitbox* hitbox: hitboxes) {
            hitbox->draw = false;
        }
    }

//    if (ImGui::Button("Show hitboxes")) {
//        spdlog::warn("showing hitboxes");
//        for(Hitbox& hitbox: hitboxes) {
//            hitbox.draw = true;
//        }
//    }
//
//    if (ImGui::Button("Hide hitboxes")) {
//        spdlog::warn("hiding hitboxes");
//        for(Hitbox& hitbox: hitboxes) {
//            hitbox.draw = false;
//        }
//    }


    ImGui::End();
}

void Walls::logHitboxes() {
    spdlog::warn("{} hitboxes loaded", hitboxes.size());
//    for(Hitbox& hitbox: hitboxes) {
//        spdlog::info("({}, {}) to ({}, {})", hitbox._min.x, hitbox._min.y, hitbox._max.x, hitbox._max.y);
//    }

    for(Hitbox* hitbox: hitboxes) {
        spdlog::info("({}, {}) to ({}, {})", hitbox->_min.x, hitbox->_min.y, hitbox->_max.x, hitbox->_max.y);
    }
}

void Walls::onCollision(Object3D *other) {

}

Walls::~Walls() {
    for(int i=0; i<hitboxes.size(); i++) {
        spdlog::info("deleted {} hitbox", i);
        delete hitboxes[i];
    }
    spdlog::info("deleted all hitboxes");
    hitboxes.clear();
}

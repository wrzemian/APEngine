//
// Created by micha on 21.04.2023.
//

#include "../include/Walls.h"
#include "../include/Hitbox.h"

#include "spdlog/spdlog.h"
#include "../include/Engine.h"


void Walls::calculateHitboxes() {
    for(Mesh mesh: _model.meshes) {
        float minX = mesh.vertices.at(0).Position.x;
        float maxX = mesh.vertices.at(0).Position.x;

        float minY = mesh.vertices.at(0).Position.y;
        float maxY = mesh.vertices.at(0).Position.y;

        float minZ = mesh.vertices.at(0).Position.z;
        float maxZ = mesh.vertices.at(0).Position.z;

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

            if(vertex.Position.z > maxZ) {
                maxZ = vertex.Position.z;
            }
            if(vertex.Position.z < minZ) {
                minZ = vertex.Position.z;
            }
        }

//        std::unique_ptr<Hitbox> hitbox = std::make_unique<Hitbox>();
 //      Hitbox hitbox; //TODO: create hitbox and save it to list
        Hitbox* hitbox = new Hitbox();
        hitbox->Create(this);

        hitbox->_min.x = minX;
        hitbox->_min.y = minY;
        hitbox->_min.z = minZ;

        hitbox->_max.x = maxX;
        hitbox->_max.y = maxY;
        hitbox->_max.z = maxZ;

        spdlog::info("Hitbox from ({}, {}, {}) to ({}, {}, {})", minX, minY, minZ, maxX, maxY, maxZ);

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

    if (ImGui::Button("SAVE WALLS")) {

        Engine::parser.SaveJSON(this->ParseToJSON(), "walls");
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

rapidjson::Document Walls::ParseToJSON() {
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
    rapidjson::Value n(_path.c_str(), d.GetAllocator());
    d.AddMember("model", n, d.GetAllocator());

    return d;
}

void Walls::loadFromJSON(Walls temp) {
    this->loadModel(temp._path);
    this->_transform._scale = temp._transform._scale;
    this->_transform._rotation = temp._transform._rotation;
    this->_transform._position = temp._transform._position;
    this->calculateHitboxes();
}

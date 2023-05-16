//
// Created by micha on 21.04.2023.
//

#include "../include/Objects/Walls.h"
#include "../include/Objects/wHitbox.h"

#include "spdlog/spdlog.h"
#include "../../include/Engine.h"


void Walls::calculateHitboxes() {
    for(Mesh mesh: _model.meshes) {
        //        std::unique_ptr<Hitbox> hitbox = std::make_unique<Hitbox>();
        auto* hitbox = new Hitbox(Hitbox::STATIC);
        hitbox->calculateFromMesh(mesh);
        hitbox->Create(this);



        if(hitbox->windowName == "hitbox #13") {
            hitbox->_min.z = -11;
            hitbox->_max.z = -10;
        }

        spdlog::info("{} from ({}, {}, {}) to ({}, {}, {})", hitbox->windowName, hitbox->_min.x, hitbox->_min.y, hitbox->_min.z,hitbox->_max.x, hitbox->_max.y, hitbox->_max.z );


        hitbox->isRendered = false;

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

//void Walls::setShader(const Shader &shader) {
//    for(auto hitbox: hitboxes) {
//       // hitbox->setShader(shader);
//    }
//}

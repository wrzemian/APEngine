#include "../include/Objects/Walls.h"
#include "../include/Objects/Hitbox.h"
#include "spdlog/spdlog.h"
#include "../../include/Engine.h"

Walls::Walls() {
    IGui::setWindowName("Walls");
}

Walls::~Walls() {
    spdlog::error("Walls destructor");
    hitboxes.clear();
}

void Walls::calculateHitboxes() {
    for (const Mesh& mesh : _model->meshes) {
        std::shared_ptr<Hitbox> hitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
        hitbox->calculateFromMesh(mesh);
        hitbox->Create(this);
        spdlog::info("Calculating hitboxes for {}", mesh._name);

        hitbox->isRendered = false;

        hitboxes.push_back(hitbox);
    }
    //logHitboxes();
}

void Walls::logHitboxes() {
    spdlog::warn("{} hitboxes loaded", hitboxes.size());
    for (const auto& hitbox : hitboxes) {
        spdlog::info("({}, {}) to ({}, {})", hitbox->_min.x, hitbox->_min.y, hitbox->_max.x, hitbox->_max.y);
    }
}

void Walls::ImGui() {
    ImGui::Begin(windowName.c_str());
    Object3D::ImGui();

    ImGui::SetWindowSize(ImVec2(300, 700));

    int x = 0;
    ImGui::SliderInt("visible hitbox", &x, 0, hitboxes.size() - 1);
    for (const auto& hitbox : hitboxes) {
        hitbox->draw = false;
    }
    hitboxes.at(x)->draw = true;

    ImGui::Checkbox("Show hitboxes", &areRendered);
    if (areRendered) {
        //spdlog::warn("showing hitboxes");
        for (const auto& hitbox : hitboxes) {
            hitbox->draw = true;
        }
    }

    if (ImGui::Button("SAVE WALLS")) {
        Engine::parser.SaveJSON(this->ParseToJSON(), "objects/walls");
    }

    ImGui::End();
}

void Walls::onCollision(Object3D* other) {
    // Implement collision handling logic for walls
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

void Walls::loadFromJSON(const Walls& temp) {
    this->loadModel(temp._path);
    this->_transform._scale = temp._transform._scale;
    this->_transform._rotation = temp._transform._rotation;
    this->_transform._position = temp._transform._position;
    this->calculateHitboxes();
}

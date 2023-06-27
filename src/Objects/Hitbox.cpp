//
// Created by micha on 02.04.2023.
//

#include "../include/Objects/Hitbox.h"

#include "../imgui_impl/imgui_impl_glfw.h"
#include "../imgui_impl/imgui_impl_opengl3.h"
#include "../../include/Engine.h"
#include "../../include/Objects/Hitbox.h"

Hitbox::Hitbox(HitboxType type) {
    _min = glm::vec3(1000, 1000, 1000);
    _max = glm::vec3(-1000, -1000, -1000);

    //spdlog::info("hitbox constructor");

    IGui::setWindowName("hitbox");

    _type = type;
   //spdlog::warn("HITBOX CONCTRUCTOR, window name = {}, tag = {}, type = {}", getWindowName(), tag, _type);

    if(type == STATIC) {
        Engine::addStaticHitbox(this);
    }
    if(type == DYNAMIC) {
        Engine::addDynamicHitbox(this);
    }
    //Engine::addImgui(this);
}

Hitbox::Hitbox(std::string fileName) {
    spdlog::info("hitbox JSON constructor");
    rapidjson::Document d = Engine::parser.openJSON(fileName);
    std::string type = d["type"].GetString();
    IGui::setWindowName("hitbox");

    if(type == "hitbox") {
        if (d["_type"].GetInt() == 0){
            _type = STATIC;
            Engine::addStaticHitbox(this);
        }
        if (d["_type"].GetInt() == 1) {
            _type = DYNAMIC;
            Engine::addDynamicHitbox(this);
        }

        _object = Engine::getObject3DById(d["object"].GetInt());
        _position = &Engine::getObject3DById(d["object"].GetInt())->_transform._position;

        _min = glm::vec3(d["minX"].GetFloat(), d["minY"].GetFloat(), d["minZ"].GetFloat());
        _max = glm::vec3(d["maxX"].GetFloat(), d["maxY"].GetFloat(), d["maxZ"].GetFloat());
        _offset = glm::vec3(d["offsetX"].GetFloat(), d["offsetY"].GetFloat(), d["offsetZ"].GetFloat());
        _color = glm::vec3(1,1,0);
    }
    else
        spdlog::error("no JSON file found");

    //spdlog::warn("HITBOX CONCTRUCTOR, window name = {}, tag = {}, type = {}", getWindowName(), _object->tag, _type);

}

Hitbox::~Hitbox() {
    spdlog::error("Hitbox object = {}, tag = {} destructor", _object->tag, tag);
    if(_type == STATIC) {
        Engine::removeStaticHitbox(this);
    }
    if(_type == DYNAMIC) {
        Engine::removeDynamicHitbox(this);
    }
}

void Hitbox::Create(Object3D* object, glm::vec3 offset) {
    _object = object;
    //spdlog::info("Create 1");
    _position = &object->_transform._position;
    //spdlog::info("Create 2");
    _offset = offset;
    _color = glm::vec3(1,1,0);

    debugShape.Initialize();
}

void Hitbox::Draw(glm::mat4 projectionView) {
    if(!draw) {
        return;
    }
    //spdlog::info("drawing hitbox {}", _windowName);

    DebugShape::shader.use();
    DebugShape::shader.setVec3("offset", _offset);
    DebugShape::shader.setVec3("color", _color);

    auto model = glm::mat4(1);
    model = glm::translate(model, *_position);
    //model = glm::scale(model, _dimensions);

    //glm::mat4 model = glm::scale(_transform->_localTransform, _dimensions);

    DebugShape::shader.setMat4("model", model);
    DebugShape::shader.setMat4("projectionView", projectionView);

    DebugShape::DrawCube(_min, _max);
}

void Hitbox::ImGui()  {
    ImGui::Begin(getWindowName().c_str());
    ImGui::SetWindowSize(ImVec2(300, 420));

    ImGui::SliderFloat("min X", &_min.x, -2.0f, 2.0f);
    ImGui::SliderFloat("min Y", &_min.y, -2.0f, 2.0f);
    ImGui::SliderFloat("min Z", &_min.z, -2.0f, 2.0f);

    ImGui::SliderFloat("max X", &_max.x, -2.0f, 2.0f);
    ImGui::SliderFloat("max Y", &_max.y, -2.0f, 2.0f);
    ImGui::SliderFloat("max Z", &_max.z, -2.0f, 2.0f);

    ImGui::SliderFloat("offset X", &_offset.x, -2, 2);
    ImGui::SliderFloat("offset Y", &_offset.y, -2, 2);
    ImGui::SliderFloat("offset Z", &_offset.z, -2, 2);

    ImGui::SliderFloat("R", &_color.x, 0, 1);
    ImGui::SliderFloat("G", &_color.y, 0, 1);
    ImGui::SliderFloat("B", &_color.z, 0, 1);

    if (ImGui::Button("Reset Offset")) {
        _offset = glm::vec3(0,0,0);
    }
    if (ImGui::Button("SAVE HITBOX")) {

        Engine::parser.SaveJSON(this->ParseToJSON(), "hitboxes/hitbox_" + std::to_string(Engine::getObject3DIndex(_object)));
    }
    if (ImGui::Button("Calculate hitbox")) {
        _min = glm::vec3(-0.001, -0.001, -0.001);
        _max = glm::vec3(0.001, 0.001, 0.001);

        calculateFromModel(*_object->_model);
    }
    ImGui::Checkbox("Draw: ", &draw);
    ImGui::End();

}

rapidjson::Document Hitbox::ParseToJSON() {
    rapidjson::Document d;
    d.SetObject();
    d.AddMember("type", "hitbox", d.GetAllocator());
    if(_type == STATIC)
        d.AddMember("_type", 0, d.GetAllocator());
    else if(_type == DYNAMIC)
        d.AddMember("_type", 1, d.GetAllocator());
    d.AddMember("object", Engine::getObject3DIndex(this->_object), d.GetAllocator());
    d.AddMember("minX", _min.x, d.GetAllocator());
    d.AddMember("minY", _min.y, d.GetAllocator());
    d.AddMember("minZ", _min.z, d.GetAllocator());
    d.AddMember("maxX", _max.x, d.GetAllocator());
    d.AddMember("maxY", _max.y, d.GetAllocator());
    d.AddMember("maxZ", _max.z, d.GetAllocator());
    d.AddMember("offsetX", _offset.x, d.GetAllocator());
    d.AddMember("offsetY", _offset.y, d.GetAllocator());
    d.AddMember("offsetZ", _offset.z, d.GetAllocator());



    return d;
}

bool Hitbox::TestForIntersection(Hitbox* other) {
    //spdlog::info("1st hitbox: ({}, {}) ({}, {})");
//    bool intersects = _position->x + _offset.x + _min.x <= other->_position->x + other->_offset.x + other->_max.x &&
//                      _position->x + _offset.x + _max.x >= other->_position->x + other->_offset.x + other->_min.x &&
//                      _position->y + _offset.y + _min.y <= other->_position->y + other->_offset.y + other->_max.y &&
//                      _position->y + _offset.y + _max.y >= other->_position->y + other->_offset.y + other->_min.y &&
//                      _position->z + _offset.z + _min.z <= other->_position->z + other->_offset.z + other->_max.z &&
//                      _position->z + _offset.z + _max.z >= other->_position->z + other->_offset.z + other->_min.z;
//
    bool intersects = checkCollision(*other);

    if(intersects) {
        _object->onCollision(other->_object);
        if(!isTrigger && !other->isTrigger)
        {
            resolveCollision(*other);
            _color.x = 1;
            _color.y = 0;
        }
        else
        {
            _object->onCollision(other->_object);
            other->_object->onCollision(_object);
            _color.x = 1;
            _color.y = 1;
        }
    } else {
        _color.x = 0;
        _color.y = 1;
    }


    return intersects;
}

// Collision detection function
bool Hitbox::checkCollision(Hitbox& other) {
    auto aMin = currentMin();
    auto aMax = currentMax();
    auto bMin = other.currentMin();
    auto bMax = other.currentMax();

    return (aMin.x <= bMax.x) && (aMax.x >= bMin.x) &&
           (aMin.y <= bMax.y) && (aMax.y >= bMin.y) &&
           (aMin.z <= bMax.z) && (aMax.z >= bMin.z);
}

// Resolve the collision by moving the object out of the other one
void Hitbox::resolveCollision(Hitbox& other) {
    glm::vec3 overlap1 = currentMax() - other.currentMin();
    glm::vec3 overlap2 = currentMin() - other.currentMax();

    glm::vec3 overlap = {
            std::abs(overlap1.x) < std::abs(overlap2.x) ? overlap1.x : overlap2.x,
            std::abs(overlap1.y) < std::abs(overlap2.y) ? overlap1.y : overlap2.y,
            std::abs(overlap1.z) < std::abs(overlap2.z) ? overlap1.z : overlap2.z,
    };

//    if(other.tag != "floor") {
//        spdlog::warn("{} min({},{},{}), max({},{},{}), {} min({},{},{}), max({},{},{})",
//                     tag,
//                     currentMin().x, currentMin().y, currentMin().z,
//                     currentMax().x, currentMax().y, currentMax().z,
//                     other.tag,
//                     other.currentMin().x, other.currentMin().y, other.currentMin().z,
//                     other.currentMax().x, other.currentMax().y, other.currentMax().z);
//        spdlog::info("overlap ({}, {}, {})", overlap.x, overlap.y, overlap.z);
//    }
    glm::vec3 overlapAbs(std::abs(overlap.x), std::abs(overlap.y), std::abs(overlap.z));

    // Find the smallest overlap
    float minOverlap = std::min({overlapAbs.x, overlapAbs.y, overlapAbs.z});

    // Move the object out of the other one along the smallest overlap axis
    if (minOverlap == overlapAbs.x) {
        _object->onCollisionX(other._object);
        _position->x -= overlap.x;
    } else if (minOverlap == overlapAbs.y) {
        _object->onCollisionY(other._object);
        _position->y -= overlap.y;
    } else if (minOverlap == overlapAbs.z) {
        _object->onCollisionZ(other._object);
        _position->z -= overlap.z;
    }
}


void Hitbox::calculateFromModel(const Model &model) {
    for(auto const& mesh: model.meshes) {
        calculateFromMesh(mesh);
    }
}

void Hitbox::calculateFromMesh(const Mesh &mesh) {
    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::lowest();

    for (const auto& vertex : mesh.vertices) {
        const auto& position = vertex.Position;
        minX = position.x < minX ? position.x : minX;
        maxX = position.x > maxX ? position.x : maxX;
        minY = position.y < minY ? position.y : minY;
        maxY = position.y > maxY ? position.y : maxY;
        minZ = position.z < minZ ? position.z : minZ;
        maxZ = position.z > maxZ ? position.z : maxZ;
    }

    _min.x = minX < _min.x ? minX : _min.x;
    _min.y = minY < _min.y ? minY : _min.y;
    _min.z = minZ < _min.z ? minZ : _min.z;

    _max.x = maxX > _max.x ? maxX : _max.x;
    _max.y = maxY > _max.y ? maxY : _max.y;
    _max.z = maxZ > _max.z ? maxZ : _max.z;
}

glm::vec3 Hitbox::currentMin() {
    return _min + _offset + *_position;
}

glm::vec3 Hitbox::currentMax() {
    return _max + _offset + *_position;
}

#include "spdlog/spdlog.h"
#include <iostream>

// Rest of the code...

void Hitbox::logFields() {
    spdlog::info("Hitbox Fields:");
    spdlog::info("- isTrigger: {}", isTrigger);
    spdlog::info("- tag: {}", tag);
    spdlog::info("- draw: {}", draw);
    spdlog::info("- _object: {}", (_object ? "not null" : "null"));
    spdlog::info("- _position: {}", (_position ? "not null" : "null"));
    spdlog::info("- _min: ({}, {}, {})", _min.x, _min.y, _min.z);
    spdlog::info("- _max: ({}, {}, {})", _max.x, _max.y, _max.z);
    spdlog::info("- _color: ({}, {}, {})", _color.x, _color.y, _color.z);
    spdlog::info("- _offset: ({}, {}, {})", _offset.x, _offset.y, _offset.z);
    spdlog::info("");
}






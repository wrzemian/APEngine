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
    if(type == STATIC) {
        Engine::addStaticHitbox(getSharedHitbox());
    }
    if(type == DYNAMIC) {
        Engine::addDynamicHitbox(getSharedHitbox());
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
            Engine::addStaticHitbox(getSharedHitbox());
        }
        if (d["_type"].GetInt() == 1) {
            _type = DYNAMIC;
            Engine::addDynamicHitbox(getSharedHitbox());
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

    spdlog::warn("HITBOX CONCTRUCTOR, window name = {}, tag = {}, type = {}", getWindowName(), _object->tag, _type);

}

Hitbox::~Hitbox() {
    if(_type == STATIC) {
        Engine::removeStaticHitbox(getSharedHitbox());
    }
    if(_type == DYNAMIC) {
        Engine::removeDynamicHitbox(getSharedHitbox());
    }
}

void Hitbox::Create(Object3D& object, glm::vec3 offset) {
    _object = object.getSharedObject();
    _position = &object._transform._position;
    _offset = offset;
    _color = glm::vec3(1,1,0);

    debugShape.Initialize();
}

void Hitbox::Draw(const glm::mat4& projectionView) const{
    if(!draw) {
        return;
    }

    DebugShape::shader.use();
    DebugShape::shader.setVec3("offset", _offset);
    DebugShape::shader.setVec3("color", _color);

    auto model = glm::mat4(1);
    model = glm::translate(model, *_position);

    DebugShape::shader.setMat4("model", model);
    DebugShape::shader.setMat4("projectionView", projectionView);

    DebugShape::DrawCube(_min, _max);
}

void Hitbox::ImGui()  {
    ImGui::Begin(getWindowName().c_str());
    ImGui::SetWindowSize(ImVec2(300, 420));

    ImGui::SliderFloat("min X", &_min.x, -20.0f, 20.0f);
    ImGui::SliderFloat("min Y", &_min.y, -20.0f, 20.0f);
    ImGui::SliderFloat("min Z", &_min.z, -20.0f, 20.0f);

    ImGui::SliderFloat("max X", &_max.x, -20.0f, 20.0f);
    ImGui::SliderFloat("max Y", &_max.y, -20.0f, 20.0f);
    ImGui::SliderFloat("max Z", &_max.z, -20.0f, 20.0f);

    ImGui::SliderFloat("offset X", &_offset.x, -5, 5);
    ImGui::SliderFloat("offset Y", &_offset.y, -5, 5);
    ImGui::SliderFloat("offset Z", &_offset.z, -5, 5);

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

bool Hitbox::TestForIntersection(Hitbox& other) {
    bool intersects = checkCollision(other);

    if(intersects) {
        _object->onCollision(*other._object);
        if(!isTrigger && !other.isTrigger)
        {
            resolveCollision(other);
            _color.x = 1;
            _color.y = 0;
        }
        else
        {
            _object->onCollision(*other._object);
            other._object->onCollision(*_object);
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
    glm::vec3 overlap = currentMin() - other.currentMax();
    glm::vec3 overlapAbs(std::abs(overlap.x), std::abs(overlap.y), std::abs(overlap.z));

    // Find the smallest overlap
    float minOverlap = std::min({overlapAbs.x, overlapAbs.y, overlapAbs.z});

    // Move the object out of the other one along the smallest overlap axis
    if (minOverlap == overlapAbs.x) {
        _object->onCollisionX(*other._object);
        _position->x -= overlap.x;
    } else if (minOverlap == overlapAbs.y) {
        _object->onCollisionY(*other._object);
        _position->y -= overlap.y;
    } else if (minOverlap == overlapAbs.z) {
        _object->onCollisionZ(*other._object);
        _position->z -= overlap.z;
    }
}


void Hitbox::calculateFromModel(const Model &model) {
    for(auto const& mesh: model.meshes) {
        calculateFromMesh(mesh);
    }
    spdlog::info("({}, {}) to ({}, {})", _min.x, _min.y, _max.x, _max.y);
}

void Hitbox::calculateFromMesh(const Mesh &mesh) {
    float minX = mesh.vertices.at(0).Position.x;
    float maxX = mesh.vertices.at(0).Position.x;

    float minY = mesh.vertices.at(0).Position.y;
    float maxY = mesh.vertices.at(0).Position.y;

    float minZ = mesh.vertices.at(0).Position.z;
    float maxZ = mesh.vertices.at(0).Position.z;

    for(auto const& vertex: mesh.vertices) {
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

    minX < _min.x ? _min.x = minX : _min.x = _min.x;
    minY < _min.y ? _min.y = minY : _min.y = _min.y;
    minZ < _min.z ? _min.z = minZ : _min.z = _min.z;

    maxX > _max.x ? _max.x = maxX : _max.x = _max.x;
    maxY > _max.y ? _max.y = maxY : _max.y = _max.y;
    maxZ > _max.z ? _max.z = maxZ : _max.z = _max.z;
}

glm::vec3 Hitbox::currentMin() const {
    return _min + _offset + *_position;
}

glm::vec3 Hitbox::currentMax() const {
    return _max + _offset + *_position;
}






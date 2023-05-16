//
// Created by micha on 02.04.2023.
//

#include "../include/Objects/Hitbox.h"

#include "../imgui_impl/imgui_impl_glfw.h"
#include "../imgui_impl/imgui_impl_opengl3.h"
#include "../../include/Engine.h"
#include "../../include/Objects/Hitbox.h"


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
    ImGui::Begin(windowName.c_str());
    ImGui::SetWindowSize(ImVec2(300, 380));

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
    ImGui::Checkbox("Draw: ", &draw);
    ImGui::End();

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
        //_object->onCollision(other->_object);
        resolveCollision(*other);
        _color.x = 1;
        _color.y = 0;
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
        _position->x -= overlap.x;
    } else if (minOverlap == overlapAbs.y) {
        _object->onCollision(other._object);
        _position->y -= overlap.y;
    } else if (minOverlap == overlapAbs.z) {
        _position->z -= overlap.z;
    }
}

Hitbox::Hitbox(HitboxType type) {
    _min = glm::vec3(-1, -1, -1);
    _max = glm::vec3(1, 1, 1);

    spdlog::warn("hitbox constructor");

    IGui::setWindowName("hitbox");

    _type = type;
    if(type == STATIC) {
        Engine::addStaticHitbox(this);
    }
    if(type == DYNAMIC) {
        Engine::addDynamicHitbox(this);
    }
    //Engine::addImgui(this);
}

Hitbox::~Hitbox() {
    if(_type == STATIC) {
       Engine::removeStaticHitbox(this);
    }
    if(_type == DYNAMIC) {
        Engine::removeDynamicHitbox(this);
    }
}

void Hitbox::calculateFromModel(const Model &model) {
    int i = 0;
    for(auto const& mesh: model.meshes) {
        i++;
        if (i==1) {
            continue;
        }
        calculateFromMesh(mesh);
    }
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

glm::vec3 Hitbox::currentMin() {
    return _min + _offset + *_position;
}

glm::vec3 Hitbox::currentMax() {
    return _max + _offset + *_position;
}






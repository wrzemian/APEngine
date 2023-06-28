//
// Created by micha on 30.03.2023.
//

#include "../include/Objects/Object3D.h"
#include "../include/Objects/Transform.h"
#include "../../include/Engine.h"
#include "../../include/AssetManager.h"

Object3D::Object3D() {

    Engine::addObject(this);
}

void Object3D::Draw(Shader &shader) {
    //shader.use();
    if(_model == nullptr) {
        spdlog::error("null model in {}", tag);
        return;
    }

    _transform.updateWorldTransform(glm::mat4(1.0f), shader);
    // shader.setMat4("model", _transform.getModel());

    _model->Draw(shader);
}

void Object3D::changeVertexPositions(const glm::vec3& offset) {
    if (_model) {
        for (auto& mesh : _model->meshes) {
            auto& vertices = mesh.vertices;
            for (size_t i = 0; i < vertices.size(); ++i) {
                vertices[i].Position += offset;
            }

            glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }
}

void Object3D::loadModel(const std::string& path) {
//    _path = path;
//    _model = AssetManager::getInstance().getModel(path);
//    calculateBoundingBox();
    _path = path;
    auto modelData = AssetManager::getInstance().getData(path);
    _model = modelData.modelPtr;
    modelMinVertex = modelData.minVertex;
    modelMaxVertex = modelData.maxVertex;
    modelMiddle = modelData.middleVertex;
    size = modelData.size;
}

void Object3D::calculateBoundingBox() {
//    auto boundingBox = Hitbox(Hitbox::BOUNDING_BOX);
//    boundingBox.Create(this);
//    boundingBox.calculateFromModel(*_model);
//
//    modelMinVertex = boundingBox._min;
//    modelMaxVertex = boundingBox._max;
//    modelMiddle = (modelMaxVertex + modelMinVertex) * 0.5f;
//    size = modelMaxVertex - modelMinVertex;

    AssetManager::ModelData modelData {_model};
    AssetManager::getInstance().calculateBoundingBox(modelData);
    modelMinVertex = modelData.minVertex;
    modelMaxVertex = modelData.maxVertex;
    modelMiddle = modelData.middleVertex;
    size = modelData.size;
}



rapidjson::Document Object3D::ParseToJSON() {
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

void Object3D::onCollisionX(Object3D *other) {
    //spdlog::warn("Object3D empty onCollisionX");
}

void Object3D::onCollisionY(Object3D *other) {
    //spdlog::warn("Object3D empty onCollisionY");
}

void Object3D::onCollisionZ(Object3D *other) {
    //spdlog::warn("Object3D empty onCollisionZ");
}

void Object3D::onCollision(Object3D *other) {
    //spdlog::warn("Object3D empty onCollision");
    this->onCollisionX(other);
    this->onCollisionY(other);
    this->onCollisionZ(other);
}

Object3D::~Object3D() {
    spdlog::error("object3d destructor");
    Engine::removeObject(this);
}

void Object3D::onCollisionExit(Object3D *other) {

}

void Object3D::loadAnimation(const std::string& daePath) {
    animated = true;
//    Model tempModel(daePath);
//    animatedModel = tempModel;
    Animation temp(daePath, &*_model);
    animation = temp;
    Animator tempA(&animation);
    animator = tempA;

}

void Object3D::unusualCollision(Object3D *other) {
    //spdlog::info("Object3D unusual collision called");
}

void Object3D::SwitchGravity(bool state) {

}

float Object3D::UpdateBlendFactor() {
    if (blendFactor < 1) {
        blendFactor += 2 * Engine::deltaTime;
        return blendFactor;
    }
    else if(blendFactor + Engine::deltaTime > 1) {
        return 1.0;
    }
}

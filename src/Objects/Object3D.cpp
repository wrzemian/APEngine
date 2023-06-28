//
// Created by micha on 30.03.2023.
//

#include "../include/Objects/Object3D.h"
#include "../include/Objects/Transform.h"
#include "../../include/Engine.h"
#include "../../include/AssetManager.h"

Object3D::Object3D() {
    IGui::setWindowName("object 3d");

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

void Object3D::ImGui() {
    ImGui::Begin(getWindowName().c_str());

    ImGui::SetWindowSize(ImVec2(300, 330));

    ImGui::SliderFloat("scale X", &_transform._scale.x, 0.0f, 2.0f);
    ImGui::SliderFloat("scale Y", &_transform._scale.y, 0.0f, 2.0f);
    ImGui::SliderFloat("scale Z", &_transform._scale.z, 0.0f, 2.0f);

    ImGui::SliderFloat("rotation X", &_transform._rotation.x, -6.5, 6.5f);
    ImGui::SliderFloat("rotation Y", &_transform._rotation.y, -6.5, 6.5f);
    ImGui::SliderFloat("rotation Z", &_transform._rotation.z, -6.5, 6.5f);

    ImGui::SliderFloat("position X", &_transform._position.x, -30.0f, 30.0f);
    ImGui::SliderFloat("position Y", &_transform._position.y, -30.0f, 30.0f);
    ImGui::SliderFloat("position Z", &_transform._position.z, -30.0f, 30.0f);

    if (ImGui::Button("Reset Scale")) {
        _transform._scale = glm::vec3(1,1,1);
    }

    if (ImGui::Button("Reset Position")) {
        _transform._position = glm::vec3(0,0,0);
    }
    if (ImGui::Button("Reset Rotation")) {
        _transform._rotation = glm::vec3(0,0,0);
    }
    if (ImGui::Button("SAVE OBJ3D")) {

        Engine::parser.SaveJSON(this->ParseToJSON(), "objects/object3D_" + std::to_string(Engine::getObject3DIndex(this)));
    }

    ImGui::End();
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

float Object3D::UpdateTimer() {
    if (animationTimer < 1) {
        animationTimer += 3*Engine::deltaTime;
    }
    return animationTimer;
}

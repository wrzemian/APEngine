//
// Created by micha on 30.03.2023.
//

#include "../include/Object3D.h"
#include "../include/Transform.h"
#include "../include/Engine.h"

Object3D::Object3D() {
    _transform = Transform();
//    windowName = fmt::format("Object3D #{}", Engine::getImguiIndex());
//    spdlog::warn("Object3d window name = {}", windowName);
//
    Engine::addObject(this);
}

void Object3D::Draw() {
    //shader.use();
    _transform.updateWorldTransform(glm::mat4(1.0f), *_shader);
    // shader.setMat4("model", _transform.getModel());
    _model.Draw(*_shader);

}

void Object3D::loadModel(const std::string &path) {
    _path = path;
    Model temp(path);
    _model = temp;
}

void Object3D::ImGui() {
    ImGui::Begin(windowName.c_str());

    ImGui::SetWindowSize(ImVec2(300, 310));

    ImGui::SliderFloat("scale X", &_transform._scale.x, 0.0f, 2.0f);
    ImGui::SliderFloat("scale Y", &_transform._scale.y, 0.0f, 2.0f);
    ImGui::SliderFloat("scale Z", &_transform._scale.z, 0.0f, 2.0f);

    ImGui::SliderFloat("rotation X", &_transform._rotation.x, -6.5, 6.5f);
    ImGui::SliderFloat("rotation Y", &_transform._rotation.y, -6.5, 6.5f);
    ImGui::SliderFloat("rotation Z", &_transform._rotation.z, -6.5, 6.5f);

    ImGui::SliderFloat("position X", &_transform._position.x, -20.0f, 20.0f);
    ImGui::SliderFloat("position Y", &_transform._position.y, -20.0f, 20.0f);
    ImGui::SliderFloat("position Z", &_transform._position.z, -20.0f, 20.0f);

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

        Engine::parser.SaveJSON(this->ParseToJSON(), "object3D_" + std::to_string(Engine::getObject3DIndex(this)));
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

void Object3D::onCollision(Object3D *other) {
    spdlog::warn("Object3D empty onCollision");
}

void Object3D::setShader(Shader *shader) {
    _shader = shader;
}

Object3D::~Object3D() {
    Engine::removeObject(this);
}

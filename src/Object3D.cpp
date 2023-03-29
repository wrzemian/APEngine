//
// Created by micha on 29.03.2023.
//

#include "../include/Object3D.h"
#include "../include/Transform.h"

Object3D::Object3D() {
    _transform = Transform();
}

Object3D::~Object3D() {

}

void Object3D::Draw(Shader &shader) {
    _transform.updateWorldTransform(glm::mat4(1.0f), shader);
   // shader.setMat4("model", _transform.getModel());
    _model.Draw(shader);

}

void Object3D::loadModel(const std::string &path) {
    Model temp(path);
    _model = temp;
}



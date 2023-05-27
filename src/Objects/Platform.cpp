//
// Created by Karol on 22.05.2023.
//

#include "../../include/Objects/Platform.h"

Platform::Platform(glm::vec3 pOrigin,glm::vec3 pTarget, float s) {
    this->positionOrigin = pOrigin;
    this->positionTarget = pTarget;
    this->speed = s;
    _transform._position = pOrigin;

    setWindowName("Moving platform");
}

void Platform::Move(float deltaTime) {
    if (moveToTarget) {
        t += deltaTime * speed;
        if (t >= 1.0f) {
            t = 1.0f;
            moveToTarget = false;
        }
    }
    else if(moveToOrigin){
        t -= deltaTime * speed;
        if (t <= 0.0f) {
            t = 0.0f;
            moveToOrigin = false;
        }
    }
    _transform._position = (1.0f - t) * positionOrigin + t * positionTarget;
}

void Platform::OnActivate() {
    moveToTarget = true;
    moveToOrigin = false;
}

void Platform::OnDeactivate() {
    moveToTarget = false;
    moveToOrigin = true;
}

void Platform::onCollision(Object3D *other) {
    //Object3D::onCollision(other);
}

void Platform::logFields() {
    spdlog::info("Platform Fields:");
    spdlog::info("- id: {}", id);
    spdlog::info("- position: ({}, {}, {})", _transform._position.x, _transform._position.y, _transform._position.z);
    spdlog::info("- rotation: ({}, {}, {})", _transform._rotation.x, _transform._rotation.y, _transform._rotation.z);
    spdlog::info("- scale: ({}, {}, {})", _transform._scale.x, _transform._scale.y, _transform._scale.z);
    spdlog::info("");
}

void Platform::ImGui() {
    ImGui::Begin(getWindowName().c_str());
    Object3D::ImGui();
    ImGui::SetWindowSize(ImVec2(300, 400));

    ImGui::Checkbox("move to origin", &moveToOrigin);
    ImGui::Checkbox("move to target", &moveToTarget);

    //TODO: delete those ifs, they are for debugging only
    if(moveToOrigin) {
        _transform._position = positionOrigin;
    }
    if(moveToTarget) {
        _transform._position = positionTarget;
    }

    ImGui::End();
}


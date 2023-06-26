//
// Created by Karol on 22.05.2023.
//

#include "../../include/Objects/Platform.h"
#include "../../include/Audio/AudioManager.h"

Platform::Platform(glm::vec3 pOrigin,glm::vec3 pTarget, float s) {
    this->positionOrigin = pOrigin;
    this->positionTarget = pTarget;
    this->speed = s;
    _transform._position = pOrigin;

    setWindowName("Platform");
    //logFields();
}

void Platform::Move(float deltaTime) {
    if (moveToTarget) {
        t += deltaTime * speed;
        if (t >= 1.0f) {
            t = 1.0f;
            moveToTarget = false;
            AudioManager::GetInstance()->PauseSound(Audio::PLATFORM_MOVE);
        }
    }
    else if(moveToOrigin){
        t -= deltaTime * speed;
        if (t <= 0.0f) {
            t = 0.0f;
            moveToOrigin = false;
            AudioManager::GetInstance()->PauseSound(Audio::PLATFORM_MOVE);
        }
    }
    _transform._position = (1.0f - t) * positionOrigin + t * positionTarget;
}

void Platform::OnActivate() {
    AudioManager::GetInstance()->PlaySound(Audio::PLATFORM_MOVE);
    moveToTarget = true;
    moveToOrigin = false;
}

void Platform::OnDeactivate() {
    AudioManager::GetInstance()->PlaySound(Audio::PLATFORM_MOVE);
    moveToTarget = false;
    moveToOrigin = true;
}

void Platform::onCollision(Object3D *other) {
    Object3D::onCollision(other);
}


void Platform::logFields() {
    spdlog::info("Platform Fields:");
    spdlog::info("- id: {}", id);
    spdlog::info("- position: ({}, {}, {})", _transform._position.x, _transform._position.y, _transform._position.z);
    spdlog::info("- origin: ({}, {}, {})", positionOrigin.x, positionOrigin.y, positionOrigin.z);
    spdlog::info("- target: ({}, {}, {})", positionTarget.x, positionTarget.y, positionTarget.z);
    spdlog::info("");
}

void Platform::ImGui() {
    ImGui::Begin(getWindowName().c_str());
    Object3D::ImGui();
    ImGui::SetWindowSize(ImVec2(300, 400));

    ImGui::Checkbox("move to origin", &moveToOrigin);
    ImGui::Checkbox("move to target", &moveToTarget);


    ImGui::End();
}



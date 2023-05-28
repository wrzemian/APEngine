//
// Created by Karol on 23.05.2023.
//

#include "../../include/Objects/Button.h"

Button::Button(Platform* p, glm::vec3 position)
{
    connectedPlatforms.push_back(p);
    _transform._position = position;
    tag = "button";
}

void Button::onCollision(Object3D *other) {
    if(other->tag == "player" || other->tag == "box")
    {
        isPushed = true;
        for (Platform* platform : connectedPlatforms) {
            platform->OnActivate();
        }
    }
}

void Button::Update(float dt) {
    if(isPushed)
    {
        t -= dt;
        if(t < 0)
        {
            for (Platform* platform : connectedPlatforms) {
                platform->OnDeactivate();
            }
            isPushed = false;
            t = timeToReset;
        }
    }
}

void Button::logFields() {
    spdlog::info("Button Fields:");
    spdlog::info("- id: {}", id);
    int i = 0;
    for (Platform* platform : connectedPlatforms) {
        spdlog::info("- connectedPlatform: {}", (connectedPlatforms[i] ? "not null" : "null"));
        i++;
    }
    spdlog::info("");
}

void Button::addPlatform(Platform *connectedPlatform) {
    connectedPlatforms.push_back(connectedPlatform);
}

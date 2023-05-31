//
// Created by Karol on 23.05.2023.
//

#include "../../include/Objects/Button.h"

Button::Button(Platform* p, glm::vec3 position)
{
    IGui::setWindowName("button");

    if(p != nullptr) {
        connectedPlatforms.push_back(p);
    }
    _transform._position = position;
    tag = "button";
}

void Button::onCollision(Object3D *other) {
    if(other->tag == "player" || other->tag == "box")
    {
        if (std::find(objectsWithContact.begin(), objectsWithContact.end(), other) == objectsWithContact.end()) {
            objectsWithContact.push_back(other);
        }
        //objectsWithContact.push_back(other);
        //std::cout << "przycisk wcisniety" << std::endl;
        isPushed = true;
        for (Platform* platform : connectedPlatforms) {
            if(platform != nullptr)
            {
                platform->OnActivate();
            }
        }
    }
}

void Button::onCollisionExit(Object3D *other) {
    Object3D::onCollisionExit(other);
    if(other->tag == "player" || other->tag == "box")
    {
        objectsWithContact.erase(std::remove(objectsWithContact.begin(), objectsWithContact.end(), other), objectsWithContact.end());
        //std::cout << "przycisk odcisniety" << std::endl;
        if(objectsWithContact.empty())
        {
            isPushed = false;
            for (Platform* platform : connectedPlatforms) {
                platform->OnDeactivate();
            }
        }
    }
}
/*
void Button::Update(float dt) {
    if(isPushed)
    {
        t -= dt;
        if(t < 0)
        {
            std::cout << "przycisk powinien sie wylaczyc" << std::endl;
            for (Platform* platform : connectedPlatforms) {
                if(platform != nullptr)
                {
                    platform->OnDeactivate();
                }
            }
            isPushed = false;
            t = timeToReset;
        }
    }
}*/

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



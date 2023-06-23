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
    if(other->tag == "player" || other->tag == "box" || other->tag == "battery")
    {
        if (std::find(objectsWithContact.begin(), objectsWithContact.end(), other) == objectsWithContact.end()) {
            objectsWithContact.push_back(other);
        }
        //objectsWithContact.push_back(other);
        //std::cout << "przycisk wcisniety" << std::endl;
        if(!isPushed)
        {
            _transform._position.y -= 0.03f;
            isPushed = true;
            bool check = false;
            for (std::shared_ptr<Button> button : conditionalButtons) {
                if(!button->isPushed && !check)
                {
                    check = true;
                }
            }
            if(!check)
            {
                for (Platform* platform : connectedPlatforms) {
                    if(platform != nullptr)
                    {
                        platform->OnActivate();
                    }
                }
                if(isWinAreaConnected)
                {
                    winArea->OnActivate();
                }
                // activate connected cable // TODO: check for bugs with multiple buttons/cables
                if(connectedCable != nullptr) {
                    connectedCable->isActive = true;
                    spdlog::info("Cable {} {} activated", connectedCable->id, connectedCable->_path);
                } else {
                    spdlog::error("button has no connected cable");
                }
            }
        }
    }
}

void Button::onCollisionExit(Object3D *other) {
    Object3D::onCollisionExit(other);
    if(other->tag == "player" || other->tag == "box" || other->tag == "battery")
    {
        objectsWithContact.erase(std::remove(objectsWithContact.begin(), objectsWithContact.end(), other), objectsWithContact.end());
        //std::cout << "przycisk odcisniety" << std::endl;
        if(objectsWithContact.empty())
        {
            if(isPushed) {
                _transform._position.y += 0.03f;
                isPushed = false;
                bool check = false;
                for (std::shared_ptr<Button> button : connectedButtons) {
                    if(button->isPushed && !check)
                    {
                        check = true;
                    }
                }
                if(!check)
                {
                    for (Platform* platform : connectedPlatforms) {
                        platform->OnDeactivate();
                    }
                    if(isWinAreaConnected)
                    {
                        winArea->OnDeactivate();
                    }
                }
                // deactivate connected cable // TODO: check for bugs with multiple buttons/cables
                if(connectedCable != nullptr) {
                    connectedCable->isActive = false;
                    spdlog::info("Cable {} {} deactivated", connectedCable->id, connectedCable->_path);

                } else {
                    spdlog::error("button has no connected cable");
                }
            }
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

void Button::addConnectedButton(std::shared_ptr<Button> buttonToConnect) {
    connectedButtons.push_back(buttonToConnect);
}

void Button::addConditionalButton(std::shared_ptr<Button> buttonToAdd) {
    conditionalButtons.push_back(buttonToAdd);
}

void Button::addWinArea(std::shared_ptr<WinArea> winAreaToAdd) {
    isWinAreaConnected = true;
    winArea = winAreaToAdd;
}

void Button::connectCable(const std::shared_ptr<Cable> &cable) {
    connectedCable = cable;
}



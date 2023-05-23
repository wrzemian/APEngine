//
// Created by Karol on 23.05.2023.
//

#include "../../include/Objects/Button.h"

Button::Button(Platform* p, glm::vec3 position)
{
    connectedPlatform = p;
    _transform._position = position;
    tag = "button";
}

void Button::onCollision(Object3D *other) {
    if(other->tag == "player")
    {
        isPushed = true;
        connectedPlatform->OnActivate();
        std::cout<<"klik i ruch" <<std::endl;
    }
}

void Button::Update(float dt) {
    if(isPushed)
    {
        t -= dt;
        if(t < 0)
        {
            connectedPlatform->OnDeactivate();
            isPushed = false;
            t = timeToReset;
        }
    }
}

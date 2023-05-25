//
// Created by Karol on 23.05.2023.
//

#include "../../include/Objects/Battery.h"

void Battery::onCollision(Object3D *other) {
    //Object3D::onCollision(other);
    if(other->tag == "floor" && _velocity.y != 0)
    {
        _velocity.y = 0;
    }
    if(other->tag == "player")
    {
        other->onCollision(this);
        //canPickUp = true;
    }
}


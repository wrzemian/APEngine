//
// Created by Karol on 28.05.2023.
//

#include "../../include/Objects/Box.h"

Box::Box() {
 //   _transform._position = glm::vec3(0,0,0);

    tag = "box";
    id = 5;
}

void Box::onCollisionY(Object3D *other) {
    MovingObject::onCollisionY(other);
    if(other->_transform._position.y > this->_transform._position.y)
    {
        if(other->tag == "player")
        {
            other->unusualCollision(this);
        }
    }
}

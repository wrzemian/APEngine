//
// Created by micha on 02.04.2023.
//

#ifndef APENGINE_HITBOX_H
#define APENGINE_HITBOX_H

#include "Transform.h"
#include "DebugShape.h"
#include "IHitbox.h"

class Hitbox : public IHitbox {

enum Axis {
    X, Y, Z, NONE
};

public:
    bool draw = true;
    Transform* _transform;
    glm::vec3* _position;

    glm::vec3 _min;
    glm::vec3 _max;

    glm::vec3 _color;

    glm::vec3 _offset;
    DebugShape debugShape;

    void Create(Transform* transform, glm::vec3 offset = glm::vec3(0));
    void Draw(glm::mat4 projectionView);
    void ImGui(std::string name = "Hitbox");

    bool TestForIntersection(Hitbox& other);
};


#endif //APENGINE_HITBOX_H

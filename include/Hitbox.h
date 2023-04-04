//
// Created by micha on 02.04.2023.
//

#ifndef APENGINE_HITBOX_H
#define APENGINE_HITBOX_H

#include "Transform.h"
#include "DebugShape.h"

class Hitbox {
public:
    bool draw = true;
    Transform* _transform;
    glm::vec3 _dimensions;
    glm::vec3 _offset;
    DebugShape debugShape;

    void Create(Transform* transform, glm::vec3 dimensions, glm::vec3 offset = glm::vec3(0));
    void Draw(glm::mat4 projectionView);
    void ImGui();
};


#endif //APENGINE_HITBOX_H

//
// Created by micha on 02.04.2023.
//

#ifndef APENGINE_HITBOX_H
#define APENGINE_HITBOX_H

#include "Transform.h"
#include "DebugShape.h"
#include "IHitbox.h"
#include "IGui.h"
#include "Object3D.h"

class Hitbox : public IGui {

enum Axis {
    X, Y, Z, NONE
};

public:
    Hitbox();

    virtual ~Hitbox();

    bool draw = true;
    Object3D* _object{};
    glm::vec3* _position{};

    glm::vec3 _min{};
    glm::vec3 _max{};

    glm::vec3 _color{};

    glm::vec3 _offset{};
    DebugShape debugShape;

    void Create(Object3D* object, glm::vec3 offset = glm::vec3(0));
    void Draw(glm::mat4 projectionView);
    void ImGui() override;

    void calculateFromMesh(const Mesh& mesh);

    bool TestForIntersection(Hitbox& other);
};


#endif //APENGINE_HITBOX_H

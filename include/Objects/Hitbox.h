//
// Created by micha on 02.04.2023.
//

#ifndef APENGINE_HITBOX_H
#define APENGINE_HITBOX_H

#include "Transform.h"
#include "DebugShape.h"
#include "../IGui.h"
#include "Object3D.h"

class Hitbox : public IGui {

enum Axis {
    X, Y, Z, NONE
};

public:
    enum HitboxType {
        DYNAMIC, STATIC
    };

    Hitbox(HitboxType type);
    Hitbox(std::string fileName);

    virtual ~Hitbox();

    glm::vec3 currentMin();
    glm::vec3 currentMax();
    bool checkCollision(Hitbox& other);
    void resolveCollision(Hitbox& other);


    bool draw = false;
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
    void calculateFromModel(const Model &model);

    bool TestForIntersection(Hitbox* other);

    rapidjson::Document ParseToJSON();

    HitboxType _type;
};


#endif //APENGINE_HITBOX_H

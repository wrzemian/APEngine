//
// Created by micha on 21.04.2023.
//

#pragma once
#include "Mesh.h"
#include "Object3D.h"
#include "Model.h"
#include "Hitbox.h"
#include <memory>

#include <vector>

class Walls : public Object3D {

public:
    Walls() {IGui::setWindowName("Walls");}

    //void setShader(const Shader& shader);

    virtual ~Walls();

    //std::vector<std::unique_ptr<Hitbox>> hitboxes;
    std::vector<Hitbox*> hitboxes;

    void calculateHitboxes();
    void logHitboxes();

    void ImGui() override;

    void onCollision(Object3D *other) override;

    rapidjson::Document ParseToJSON();

    void loadFromJSON(Walls temp);
};



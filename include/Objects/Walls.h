#pragma once
#include "Mesh.h"
#include "Object3D.h"
#include "Model.h"
#include "Hitbox.h"
#include <memory>
#include <vector>

class Walls : public Object3D {
public:
    Walls();
    virtual ~Walls();

    std::vector<std::shared_ptr<Hitbox>> hitboxes;

    void calculateHitboxes();
    void logHitboxes();

    void ImGui() override;

    void onCollision(Object3D* other) override;

    rapidjson::Document ParseToJSON();

    void loadFromJSON(const Walls& temp);
};

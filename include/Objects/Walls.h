#pragma once
#include "Mesh.h"
#include "Object3D.h"
#include "Model.h"
#include "Hitbox.h"
#include "Button.h"
#include <memory>
#include <vector>

class Walls : public Object3D {
public:
    Walls();
    virtual ~Walls();
    bool areRendered = false;

    std::vector<std::shared_ptr<Hitbox>> hitboxes;
    std::vector<std::shared_ptr<Platform>> movingPlatforms;
    std::vector<std::shared_ptr<Button>> buttons;

    std::unordered_map<int, glm::vec3> targetPositions;

    void assignTargetsAndPlatforms();

    void calculateHitboxes();
    void logHitboxes();
    void logNewObjects();
    void ImGui() override;

    void onCollision(Object3D* other) override;

    rapidjson::Document ParseToJSON();

    void loadFromJSON(const Walls& temp);
};

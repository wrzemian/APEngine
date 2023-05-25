#pragma once
#include "Mesh.h"
#include "Object3D.h"
#include "Model.h"
#include "Hitbox.h"
#include "Button.h"
#include <memory>
#include <vector>

class Level : public Object3D {
public:
    Level();
    virtual ~Level();
    bool areRendered = false;

    std::unordered_map<int, glm::vec3> targetPositions;
    std::vector<std::shared_ptr<Hitbox>> hitboxes;
    std::vector<std::shared_ptr<Platform>> movingPlatforms;
    std::vector<std::shared_ptr<Button>> buttons;

    void calculateHitboxes();
    void logHitboxes();

    void ImGui() override;

    void onCollision(std::shared_ptr<Object3D> other) override;

    rapidjson::Document ParseToJSON();

    void loadFromJSON(const Level& temp);

    void assignTargetsToPlatforms();

    void assignButtonsToPlatforms();

    void loadLevel(const std::string &fileName);
};

#pragma once
#include "Mesh.h"
#include "Object3D.h"
#include "Model.h"
#include "Hitbox.h"
#include "Button.h"
#include "Battery.h"
#include <memory>
#include <vector>

class Walls : public Object3D {
public:
    Walls();
    virtual ~Walls();
    bool buttonsImgui = false;
    bool platformsImgui = false;
    bool batteriesImgui = false;
    bool boxesImgui = false;

    std::vector<std::shared_ptr<Hitbox>> hitboxes;
    std::vector<std::shared_ptr<Platform>> movingPlatforms;
    std::vector<std::shared_ptr<Button>> buttons;
    std::vector<std::shared_ptr<Object3D>> boxes; //TODO: add class Box
    std::vector<std::shared_ptr<Battery>> batteries;

    std::unordered_map<int, glm::vec3> targetPositions;

    void assignTargetsAndPlatforms();
    void setShader(Shader* shader);

    void calculateHitboxes();
    void logHitboxes();
    void logNewObjects();
    void ImGui() override;

    void onCollision(Object3D* other) override;

    rapidjson::Document ParseToJSON();

    void loadFromJSON(const Walls& temp);

    void Draw();

    Model staticModel;
};
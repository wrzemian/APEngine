#pragma once
#include "Mesh.h"
#include "Object3D.h"
#include "Model.h"
#include "Hitbox.h"
#include "Button.h"
#include "Battery.h"
#include "WinArea.h"
#include <memory>
#include <vector>

class Level : public Object3D {
public:
    Level();
    virtual ~Level();
    bool buttonsImgui = false;
    bool platformsImgui = false;
    bool batteriesImgui = false;
    bool boxesImgui = false;

    std::vector<std::shared_ptr<Hitbox>> hitboxes;
    std::vector<std::shared_ptr<Platform>> movingPlatforms;
    std::vector<std::shared_ptr<Button>> buttons;
    std::vector<std::shared_ptr<Object3D>> boxes; //TODO: add class Box
    std::vector<std::shared_ptr<Battery>> batteries;

    std::shared_ptr<WinArea> winArea = nullptr;

    std::unordered_map<int, glm::vec3> targetPositions;

    void assignTargetsAndPlatforms();

    void calculateHitboxes();
    void logHitboxes();
    void logNewObjects();
    void ImGui() override;

    void onCollision(Object3D* other) override;

    rapidjson::Document ParseToJSON();

    void loadFromJSON(const Level& temp);

    void Draw(Shader &shader);

    Model staticModel;

    glm::vec3 cameraOffset = glm::vec3(10.0f,10.0f,17.0f);
    glm::vec3 playerGrabberStartingPos = glm::vec3(-5,1,-1.0f);
    glm::vec3 playerJumperStartingPos = glm::vec3(-5,1,0);
};
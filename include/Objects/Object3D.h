//
// Created by micha on 30.03.2023.
//

#pragma once
#include "Transform.h"
#include "Model.h"
#include "../Shader.h"
#include "../IGui.h"
#include "document.h"
#include "../Animations2/Animation.h"
#include "../Animations2/Animator.h"

class Object3D : public IGui {
public:
    int levelId = -1;
    Object3D();
    virtual ~Object3D();
    int id = -1;
    bool rendered = true;
    bool animated = false;
    bool emissive = false;

    glm::vec3 modelMinVertex{};
    glm::vec3 modelMaxVertex{};
    glm::vec3 modelMiddle{};
    glm::vec3 size = {};

    std::string tag = "floor";

    std::string type = "none";


    virtual void Draw(Shader &shader);

    void ImGui();
    void loadModel(const std::string& path);

    void loadAnimation(const std::string& daePath);

    virtual void onCollisionX(Object3D* other);
    virtual void onCollisionY(Object3D* other);
    virtual void onCollisionZ(Object3D* other);
    virtual void onCollision(Object3D* other);
    virtual void onCollisionExit(Object3D* other);
    virtual rapidjson::Document ParseToJSON();


    virtual void unusualCollision(Object3D* other);

    Transform _transform;
    std::shared_ptr<Model> _model = nullptr;

    Animation animation;
    Animation currentAnimation;
    Animation previousAnimation;
    Animator animator;
    Shader* _shader = nullptr;

    std::string _path;

    bool canBePickedUp = true;

    float animationTimer = 0;
    float UpdateTimer();

    void calculateBoundingBox();

    void changeVertexPositions(const glm::vec3 &offset);

    virtual void SwitchGravity(bool state);
};

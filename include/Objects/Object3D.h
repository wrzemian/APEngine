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

    glm::vec3 boundingBoxMin{};
    glm::vec3 boundingBoxMax{};
    glm::vec3 size = {};

    glm::vec3 getMiddle();

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

    Model animatedModel;
    Animation animation;
    Animator animator;
    Shader* _shader = nullptr;

    std::string _path;

    void calculateBoundingBox();
};

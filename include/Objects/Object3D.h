//
// Created by micha on 30.03.2023.
//

#pragma once
#include "Transform.h"
#include "Model.h"
#include "../Shader.h"
#include "../IGui.h"
#include "document.h"

class Object3D : public IGui {
public:
    Object3D();
    virtual ~Object3D();
    int id = -1;
    bool rendered = true;

    glm::vec3 boundingBoxMin{};
    glm::vec3 boundingBoxMax{};

    std::string tag = "floor";

    virtual void Draw(Shader &shader);

    void ImGui();
    void loadModel(const std::string& path);

    virtual void onCollisionX(Object3D* other);
    virtual void onCollisionY(Object3D* other);
    virtual void onCollisionZ(Object3D* other);
    virtual void onCollision(Object3D* other);
    virtual void onCollisionExit(Object3D* other);
    virtual rapidjson::Document ParseToJSON();

    Transform _transform;
    std::shared_ptr<Model> _model = nullptr;
    std::string _path;
};

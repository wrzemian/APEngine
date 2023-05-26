//
// Created by micha on 30.03.2023.
//

#pragma once
#include "Transform.h"
#include "Model.h"
#include "../Shader.h"
#include "../IGui.h"
#include "document.h"

class MovingObject;
class Ant;

class Object3D : public IGui, public std::enable_shared_from_this<Object3D> {
public:
    Object3D();
    virtual ~Object3D();

    std::string tag = "floor";

    void Draw();
    void setShader(const std::shared_ptr<Shader>& shader);

    void ImGui() override;
    void loadModel(const std::string& path);

    virtual void onCollisionX(Object3D& other);
    virtual void onCollisionY(Object3D& other);
    virtual void onCollisionZ(Object3D& other);
    virtual void onCollision( Object3D& other);
    virtual rapidjson::Document ParseToJSON();

    Transform _transform;
    std::shared_ptr<Model> _model;
    std::shared_ptr<Shader> _shader;
    std::string _path;
};

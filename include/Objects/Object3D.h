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

    Shader* _shader;

    Model* _model;
    Transform _transform;
    std::string _path;

    void Draw();
    void setShader(Shader* shader);
    void loadModel(std::string const &path);

    virtual void onCollision(Object3D* other);

    virtual void ImGui();

    virtual rapidjson::Document ParseToJSON();
};
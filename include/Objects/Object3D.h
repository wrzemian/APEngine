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

    void Draw();
    void setShader(Shader* shader);

    void ImGui();
    void loadModel(const std::string& path);

    virtual void onCollision(Object3D* other);
    virtual rapidjson::Document ParseToJSON();

    Transform _transform;
    std::shared_ptr<Model> _model;
    Shader* _shader;
    std::string _path;
};

//
// Created by micha on 30.03.2023.
//

#pragma once
#include "Transform.h"
#include "Model.h"
#include "Shader.h"
#include "IGui.h"

class Object3D : public IGui {

public:
    Object3D();
    virtual ~Object3D();

    Model _model;
    Transform _transform;

    void Draw(Shader &shader);
    void loadModel(std::string const &path);

    virtual void onCollision(Object3D* other);

    virtual void ImGui();
};
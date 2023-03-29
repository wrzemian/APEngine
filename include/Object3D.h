//
// Created by micha on 29.03.2023.
//

#pragma once
#include "Transform.h"
#include "Model.h"
#include "Shader.h"

class Object3D {

public:
    Object3D();
    virtual ~Object3D();

    Model _model;
    Transform _transform;

    void Draw(Shader &shader);
    void loadModel(std::string const &path);
};



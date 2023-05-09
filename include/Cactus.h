//
// Created by Karol on 27.04.2023.
//

#pragma once

#include "Object3D.h"
#include "random.hpp"
using Random = effolkronium::random_static;

class Cactus : public Object3D{

private:
    typedef Object3D super;

    //Object3D cactusArm;

public:
    Cactus();

    ~Cactus();

    void initCactus();

    void randomizeSize(float min, float max);

    void randomizeRotation();

};
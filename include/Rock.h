//
// Created by Karol on 27.04.2023.
//

#pragma once

#include "Objects/Object3D.h"
#include "random.hpp"
using Random = effolkronium::random_static;

class Rock : public Object3D{

private:
    typedef Object3D super;

public:
    Rock();  

    void initRock();

    void randomizeSize(float min, float max);

    void randomizeRotation();

};
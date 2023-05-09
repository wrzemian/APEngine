//
// Created by Karol on 27.04.2023.
//

#include "../include/Rock.h"
#include "../include/Engine.h"

void Rock::initRock() {
    loadModel("../../res/models/rock/rock.obj");
}

void Rock::randomizeSize(float min, float max)
{
    _transform._scale.x = Random::get<Random::common>(min, max);
    _transform._scale.y = Random::get<Random::common>(min, max);
    _transform._scale.z = Random::get<Random::common>(min, max);
}

void Rock::randomizeRotation()
{
    srand(time(NULL));
    int range = 360 - 0 + 1;
    _transform._rotation.x =  std::rand() % range + 0;
    _transform._rotation.y =  std::rand() % range + 0;
    _transform._rotation.z =  std::rand() % range + 0;
}

Rock::Rock() {
    IGui::setWindowName("rock object");
}

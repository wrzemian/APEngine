//
// Created by Karol on 27.04.2023.
//

#include "../../include/Background/Cactus.h"

void Cactus::initCactus()
{
    loadModel("../../res/models/World/cactus/kaktus.obj");
   // cactusArm.loadModel("../../res/models/cactus/kaktus_odnoga1.obj");
}

void Cactus::randomizeSize(float min, float max)
{
    _transform._scale.x = Random::get<Random::common>(min, max);
    _transform._scale.y = Random::get<Random::common>(min, max);
    _transform._scale.z = Random::get<Random::common>(min, max);
}

void Cactus::randomizeRotation()
{
    _transform._rotation.y = Random::get<Random::common>(1, 360);
}

Cactus::Cactus() {
    IGui::setWindowName("cactus object");
}

Cactus::~Cactus() {
    //std::cout <<"lord have mercy"<<std::endl;
}

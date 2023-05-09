//
// Created by Karol on 27.04.2023.
//

#include "../include/BackgroundTile.h"

void BackgroundTile::initBackgroundTile()
{
    loadModel("../../res/models/world/PUSTYNIA_z_torami.obj");
    //_transform._scale.x = 0.6f;
    //_transform._scale.y = 0.6f;
    //_transform._scale.z = 0.6f;
    //_transform._position.x = -1200;
    _transform._position.y = 0;
    //_transform._position.z = 100;
    _transform._rotation.y = -1.57079632679f;
}

void BackgroundTile::Move(float step)
{
    _transform._position.x += step;
    for(int i = 0 ; i < cactuses.size(); i++)
    {
        cactuses[i]->_transform._position.x += step;
    }
    for(int i = 0 ; i < rocks.size(); i++)
    {
        rocks[i]->_transform._position.x += step;
    }
}

void BackgroundTile::SetBoundaries(int maxX,int minX,int maxZ,int minZ)
{
    maxBoundaryX = maxX;
    minBoundaryX = minX;
    maxBoundaryZ = maxZ;
    minBoundaryZ = minZ;
}

void BackgroundTile::SetAmountToGenerate(int maxCact,int minCact,int maxR,int minR)
{
    maxCactuses = maxCact;
    minCactuses = minCact;
    maxRocks = maxR;
    minRocks = minR;
}

void BackgroundTile::GenerateRandomObjects()
{
    int cactusesAmount = Random::get(minCactuses, maxCactuses);
    int rocksAmount = Random::get(minRocks, maxRocks);

    for(int i = 0 ;i < cactusesAmount;i++)
    {
        //Cactus cactus;
        Cactus *cactus = new Cactus();
        cactuses.push_back(cactus);
        cactuses[i]->_shader = _shader;
        cactuses[i]->_transform._position.x = _transform._position.x + Random::get<Random::common>(minBoundaryX, maxBoundaryX);
        cactuses[i]->_transform._position.z = _transform._position.z + Random::get<Random::common>(minBoundaryZ, maxBoundaryZ);
        cactuses[i]->initCactus();
        cactuses[i]->randomizeRotation();
        cactuses[i]->randomizeSize(1,2);
    }
    for(int i = 0 ;i < rocksAmount;i++)
    {
        Rock *rock = new Rock();
        rocks.push_back(rock);
        rocks[i]->_shader = _shader;
        rocks[i]->_transform._position.x = Random::get<Random::common>(minBoundaryX, maxBoundaryX);
        rocks[i]->_transform._position.z = Random::get<Random::common>(minBoundaryZ, maxBoundaryZ);
        rocks[i]->initRock();
        rocks[i]->randomizeRotation();
        rocks[i]->randomizeSize(1,2);
    }
}

void BackgroundTile::RegenerateRandomObjects()
{
    cactuses.clear();
    rocks.clear();
}

BackgroundTile::~BackgroundTile() {
    for (Cactus *cactus : cactuses) {
        delete cactus;
    }

    for (Rock *rock : rocks) {
        delete rock;
    }
}

BackgroundTile::BackgroundTile() {
    IGui::setWindowName("Background tile");
}

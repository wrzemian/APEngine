//
// Created by Karol on 27.04.2023.
//

#include "../include/BackgroundTile.h"

void BackgroundTile::initBackgroundTile()
{
    loadModel("../../res/models/World/terrain/pustynia.obj");
    //_transform._scale.x = 0.6f;
    _transform._scale.y = 0.1f;
    //_transform._scale.z = 0.6f;
    //_transform._position.x = -1200;
    _transform._position.y = 0;
    _transform._position.z = 100;
    _transform._rotation.y = 1.57079632679f;
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
    cactusesAmount = Random::get(minCactuses, maxCactuses);
    rocksAmount = Random::get(minRocks, maxRocks);

    for(int i = 0 ;i < cactusesAmount;i++)
    {
        //Cactus cactus;
        Cactus *cactus = new Cactus();
        cactus->isRendered = false;
        cactuses.push_back(cactus);
        cactuses[i]->_shader = _shader;
        cactuses[i]->_transform._position.x = _transform._position.x + Random::get<Random::common>(minBoundaryX, maxBoundaryX);
        cactuses[i]->_transform._position.z = _transform._position.z + Random::get<Random::common>(minBoundaryZ, maxBoundaryZ);
        cactuses[i]->_transform._position.y = 0.2;
        cactuses[i]->initCactus();
        cactuses[i]->randomizeRotation();
        cactuses[i]->randomizeSize(1,1.5);
    }
    for(int i = 0 ;i < rocksAmount;i++)
    {
        Rock *rock = new Rock();
        rock->isRendered = false;
        rocks.push_back(rock);
        rocks[i]->_shader = _shader;
        rocks[i]->_transform._position.x = _transform._position.x + Random::get<Random::common>(minBoundaryX, maxBoundaryX);
        rocks[i]->_transform._position.z = _transform._position.z + Random::get<Random::common>(minBoundaryZ, maxBoundaryZ);
        rocks[i]->_transform._position.y = 0.2;
        rocks[i]->initRock();
        rocks[i]->randomizeRotation();
        rocks[i]->randomizeSize(1,2);
    }
}

void BackgroundTile::RegenerateRandomObjects()
{
    for(int i = 0 ;i < cactusesAmount;i++)
    {
        cactuses[i]->_transform._position.x = _transform._position.x + Random::get<Random::common>(minBoundaryX, maxBoundaryX);
        cactuses[i]->_transform._position.z = _transform._position.z + Random::get<Random::common>(minBoundaryZ, maxBoundaryZ);
        cactuses[i]->randomizeRotation();
        cactuses[i]->randomizeSize(1,1.5);
    }
    for(int i = 0 ;i < rocksAmount;i++)
    {
        rocks[i]->_transform._position.x = _transform._position.x + Random::get<Random::common>(minBoundaryX, maxBoundaryX);
        rocks[i]->_transform._position.z = _transform._position.z + Random::get<Random::common>(minBoundaryZ, maxBoundaryZ);
        rocks[i]->randomizeRotation();
        rocks[i]->randomizeSize(1,2);
    }
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

//
// Created by Karol on 27.04.2023.
//

#pragma once

#include "../Objects/Object3D.h"
#include "Cactus.h"
#include "Rock.h"
class BackgroundTile : public Object3D{
private:
    typedef Object3D super;

    std::vector<Cactus*> cactuses;
    std::vector<Rock*> rocks;
    int maxCactuses;
    int minCactuses;
    int maxRocks;
    int minRocks;
    int maxBoundaryX;
    int minBoundaryX;
    int maxBoundaryZ;
    int minBoundaryZ;
    int cactusesAmount;
    int rocksAmount;
public:
    BackgroundTile();

    ~BackgroundTile();

    void initBackgroundTile();  // used to load tile obj

    void Move(float step);  //moves tile and everything on it

    void SetBoundaries(int maxX,int minX,int maxZ,int minZ);

    void SetAmountToGenerate(int maxCact,int minCact,int maxR,int minR);

    void GenerateRandomObjects();

    void RegenerateRandomObjects();
};



//
// Created by Karol on 27.04.2023.
//

#pragma once

#include "../Objects/Object3D.h"
#include "BackgroundTile.h"

class Background {
private:
    std::vector<BackgroundTile*> backgroundTiles;
    float interval = 10.0f;
    float leftBoarder = 100.0f; //the boarder after which tile moves back to the start of the tiles
    int tilesNumber;
public:
    void initBackground(int tilesAmount,float maxLeft, float shiftAmount,Shader* shader);  // used to load tile obj

    void Move(float step);  //moves tiles

};


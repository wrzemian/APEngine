//
// Created by Karol on 27.04.2023.
//

#include "../include/Background.h"

void Background::initBackground(int tilesAmount, float maxLeft, float shiftAmount, Shader* shader) {
    leftBoarder = maxLeft;
    tilesNumber = tilesAmount;
    interval = shiftAmount;
    for(int i = -1 ;i < tilesAmount-1;i++)
    {
        // xPosition = i * interval;
        //create new tiles here
        std::cout << "stworzony tile" << std::endl;
        BackgroundTile *bTile = new BackgroundTile();
        backgroundTiles.push_back(bTile);
        backgroundTiles[i+1]->_transform._position.x += interval * i;
        std::cout << interval * i << std::endl;
        backgroundTiles[i+1]->setShader(shader);
        backgroundTiles[i+1]->initBackgroundTile();
        backgroundTiles[i+1]->SetBoundaries(100,0,-1,-100);
        backgroundTiles[i+1]->SetAmountToGenerate(5,2,5,2);
        backgroundTiles[i+1]->GenerateRandomObjects();
    }
}

void Background::Move(float step) {
    for(BackgroundTile* tile : backgroundTiles)
    {
        tile->Move(step);
        if(tile->_transform._position.x < leftBoarder)
        {
            tile->_transform._position.x += interval*tilesNumber;
            tile->RegenerateRandomObjects();
        }
    }
}



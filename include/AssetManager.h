//
// Created by micha on 22.05.2023.
//

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "../include/Objects/Model.h"

enum ModelName{
    CRANK,
    MICHEL,
    LEVEL1,
    CACTUS,
    ROCK
};

namespace AssetManager{



    Model* requestModel(ModelName modelName);
    Model* requestModel(std::string path);

    //void loadModels();

};

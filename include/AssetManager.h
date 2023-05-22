//
// Created by micha on 22.05.2023.
//

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "../include/Objects/Model.h"

using std::string;

enum ModelName{
    CRANK,
    MICHEL,
    LEVEL1,
    CACTUS,
    ROCK
};

class AssetManager{

private:
    std::unordered_map<ModelName, string> paths = {
            {CRANK, "../../res/models/Players/Cr4nk/crank.obj"},
            {MICHEL, "../../res/models/Players/Mich3l/michel_final.obj"},
            {LEVEL1, "../../res/models/Levels/1/1level.obj"},
            {CACTUS, "../../res/models/World/cactus/kaktus.obj"},
            {ROCK, "../../res/models/World/rock/rock.obj"},
    };

    std::unordered_map<string, Model*> models;

public:
    Model* requestModel(ModelName modelName);
    void loadModels();

};

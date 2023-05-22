//
// Created by micha on 22.05.2023.
//

#include "../include/AssetManager.h"
#include "spdlog/spdlog.h"


std::unordered_map <ModelName, std::string> paths = {
        {CRANK,  "../../res/models/Players/Cr4nk/crank.obj"},
        {MICHEL, "../../res/models/Players/Mich3l/michel_final.obj"},
        {LEVEL1, "../../res/models/Levels/1/1level.obj"},
        {CACTUS, "../../res/models/World/cactus/kaktus.obj"},
        {ROCK,   "../../res/models/World/rock/rock.obj"},
};

std::unordered_map<std::string, Model *> models;


//void AssetManager::loadModels() {
//    for (const std::pair<const ModelName, const std::string> &p: paths) {
//        spdlog::info("Loading path {}", p.second);
//
//        Model *model = new Model(p.second);
//        models[p.second] = model;
//    }
//}

Model *AssetManager::requestModel(std::string path) {
    if (models.count(path) > 0) {
        return models[path];
    }
    spdlog::info("Model at \"{}\" not found", path);
    Model *model = new Model(path);
    models[path] = model;
}

Model *AssetManager::requestModel(ModelName modelName) {
    return requestModel(paths[modelName]);
}




//
// Created by micha on 22.05.2023.
//

#include "../include/AssetManager.h"
#include "spdlog/spdlog.h"

void AssetManager::loadModels() {
    for(const std::pair<const ModelName, const string>& p: paths) {
        spdlog::info("Loading path {}", p.second);

        Model* model = new Model(p.second);
        models[p.second] = model;
    }
}

Model *AssetManager::requestModel(ModelName modelName) {
    return NULL;
}


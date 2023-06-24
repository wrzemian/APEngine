#include "../include/AssetManager.h"

AssetManager& AssetManager::getInstance() {
    static AssetManager instance;
    return instance;
}

AssetManager::ModelPtr AssetManager::getModel(const std::string& path) {
//    spdlog::info("requested model at {}", path);

    if (models.find(path) != models.end()) {
//        spdlog::info("returning model");
        return models[path];
    }

    ModelPtr model = std::make_shared<Model>(path);
    models[path] = model;
//    spdlog::info("returning model");
    return model;
}

void AssetManager::clearModels() {
    models.clear();
}

AssetManager::AssetManager() {}

AssetManager::~AssetManager() {
    clearModels();
}

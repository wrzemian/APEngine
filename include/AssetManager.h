#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <unordered_map>
#include <memory>
#include "Objects/Model.h"

class AssetManager {
public:
    typedef std::shared_ptr<Model> ModelPtr;

    static AssetManager& getInstance();

    ModelPtr getModel(const std::string& path);
    void clearModels();

private:
    std::unordered_map<std::string, ModelPtr> models;

    AssetManager();
    ~AssetManager();

    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;
};

#endif  // ASSETMANAGER_H

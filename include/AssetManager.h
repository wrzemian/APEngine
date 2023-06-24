#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <unordered_map>
#include <memory>
#include "Objects/Model.h"

class AssetManager {

public:
    typedef std::shared_ptr<Model> ModelPtr;

    struct ModelData {
        ModelPtr modelPtr;
        glm::vec3 minVertex;
        glm::vec3 maxVertex;
        glm::vec3 middleVertex;
        glm::vec3 size;
    };


    static AssetManager& getInstance();

    ModelPtr getModel(const std::string& path);
    ModelData getData(const std::string& path);

    void clearModels();

    void calculateFromMesh(const Mesh &mesh, ModelData& data);
    void calculateBoundingBox(ModelData& data);

private:

    std::unordered_map<std::string, ModelData> modelsData;

    AssetManager();
    ~AssetManager();

    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;
};

#endif  // ASSETMANAGER_H

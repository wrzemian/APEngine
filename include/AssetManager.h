#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <unordered_map>
#include <memory>
#include "Objects/Model.h"

class AssetManager {

public:
    using ModelPtr = std::shared_ptr<Model>;
    using floatLimit = std::numeric_limits<float>;

    struct ModelData {
        ModelPtr modelPtr;
        glm::vec3 minVertex = {floatLimit::max(), floatLimit::max(), floatLimit::max()};
        glm::vec3 maxVertex = {floatLimit::lowest(), floatLimit::lowest(), floatLimit::lowest()};
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

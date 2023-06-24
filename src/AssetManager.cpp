#include "../include/AssetManager.h"

AssetManager& AssetManager::getInstance() {
    static AssetManager instance;
    return instance;
}

AssetManager::ModelPtr AssetManager::getModel(const std::string& path) {
    if (modelsData.find(path) != modelsData.end()) {
        return modelsData[path].modelPtr;
    }

    ModelPtr model = std::make_shared<Model>(path);
    modelsData[path].modelPtr = model;
    return model;
}

void AssetManager::clearModels() {
    modelsData.clear();
}

AssetManager::AssetManager() {}

AssetManager::~AssetManager() {
    clearModels();
}

AssetManager::ModelData AssetManager::getData(const std::string &path) {
    auto it = modelsData.find(path);
    if (it != modelsData.end()) {
        return it->second;
    }

    ModelPtr model = std::make_shared<Model>(path);
    ModelData& data = modelsData[path];
    data.modelPtr = model;
    calculateBoundingBox(data);
    return data;
}


void AssetManager::calculateFromMesh(const Mesh &mesh, ModelData& data) {
    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::lowest();

    for (const auto& vertex : mesh.vertices) {
        const auto& position = vertex.Position;
        minX = std::min(position.x, minX);
        maxX = std::max(position.x, maxX);
        minY = std::min(position.y, minY);
        maxY = std::max(position.y, maxY);
        minZ = std::min(position.z, minZ);
        maxZ = std::max(position.z, maxZ);
    }

    data.minVertex.x = minX < data.minVertex.x ? minX : data.minVertex.x;
    data.minVertex.y = minY < data.minVertex.y ? minY : data.minVertex.y;
    data.minVertex.z = minZ < data.minVertex.z ? minZ : data.minVertex.z;

    data.maxVertex.x = maxX > data.maxVertex.x ? maxX : data.maxVertex.x;
    data.maxVertex.y = maxY > data.maxVertex.y ? maxY : data.maxVertex.y;
    data.maxVertex.z = maxZ > data.maxVertex.z ? maxZ : data.maxVertex.z;


}


void AssetManager::calculateBoundingBox(ModelData& data) {
    for(auto const& mesh: data.modelPtr->meshes) {
        calculateFromMesh(mesh, data);
    }
    data.middleVertex = (data.maxVertex + data.minVertex) * 0.5f;

    data.size = data.maxVertex - data.minVertex;
}


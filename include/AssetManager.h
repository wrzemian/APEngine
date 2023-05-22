#include <unordered_map>
#include "Objects/Model.h"
#include "spdlog/spdlog.h"


class AssetManager {
public:
    static AssetManager& getInstance() {
        static AssetManager instance;
        return instance;
    }

    Model* getModel(const std::string& path) {
        // Check if the model is already loaded
        if (models.find(path) != models.end()) {
            return models[path];
        }
        spdlog::info("model {} not found, loading", path);

        // Model is not loaded, load it and store in the map
        Model* model = new Model(path);
        models[path] = model;
        return model;
    }

private:
    std::unordered_map<std::string, Model*> models;

    AssetManager() {}
    ~AssetManager() {}

    // Disable copy constructor and assignment operator
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;
};
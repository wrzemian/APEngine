#include <unordered_map>
#include <memory>

class AssetManager {
public:
    typedef std::shared_ptr<Model> ModelPtr;

    static AssetManager& getInstance() {
        static AssetManager instance;
        return instance;
    }

    ModelPtr getModel(const std::string& path) {
        if (models.find(path) != models.end()) {
            return models[path];
        }

        ModelPtr model = std::make_shared<Model>(path);
        models[path] = model;
        return model;
    }

    void clearModels() {
        models.clear();
    }

private:
    std::unordered_map<std::string, ModelPtr> models;

    AssetManager() {}
    ~AssetManager() {
        clearModels();
    }

    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;
};

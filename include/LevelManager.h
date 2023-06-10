#include <fstream>
#include <sstream>
#include "Objects/Level.h"
#include "spdlog/spdlog.h"

class LevelManager {
public:
    typedef std::shared_ptr<Level> LevelPtr;

    static LevelManager& getInstance() {
        static LevelManager instance;
        return instance;
    }

    void loadAllLevels(const std::string& pathToFile) {
        std::ifstream file(pathToFile);
        if (!file) {
            spdlog::error("Could not open file: {}", pathToFile);
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            LevelPtr level = std::make_shared<Level>();
            level->loadModel(line);
            levels.push_back(level);
        }
    }

    std::vector<LevelPtr>& getLevels() {
        return levels;
    }

    void clearLevels() {
        levels.clear();
    }

private:
    std::vector<LevelPtr> levels;

    LevelManager() {}
    ~LevelManager() {
        clearLevels();
    }

    LevelManager(const LevelManager&) = delete;
    LevelManager& operator=(const LevelManager&) = delete;
};

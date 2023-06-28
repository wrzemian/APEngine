#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <fstream>
#include <sstream>
#include "Objects/Level.h"
#include "spdlog/spdlog.h"
#include "Engine.h"

class LevelManager {
public:
    typedef std::shared_ptr<Level> LevelPtr;
    typedef std::shared_ptr<Hitbox> HitboxPtr;

    static LevelManager& getInstance();

    void loadAllLevels(const std::string& pathToFile);
    void loadAllLevelsData(const std::string& pathToFile);
    std::vector<LevelPtr>& getLevels();
    void clearLevels();
    std::shared_ptr<Level> getCurrentLevel();
    bool nextLevel();

    int currentLevel = 0;

private:
    void loadLocomotive();
    void loadInvisibleWalls();

    std::shared_ptr<Object3D> locomotive = nullptr;
    std::vector<LevelPtr> levels;
    std::vector<HitboxPtr> hitboxes;
    std::vector<std::string> successfulPaths;
    std::vector<std::string> failedPaths;
    int levelCount = 0;

    LevelManager();
    ~LevelManager();

    LevelManager(const LevelManager&) = delete;
    LevelManager& operator=(const LevelManager&) = delete;
};

#endif  // LEVELMANAGER_H

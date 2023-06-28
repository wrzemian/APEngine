#include "../include/LevelManager.h"

const glm::vec3 defaultPosition = {-7.039000034332275, 4.692999839782715, 4.35699987411499};

LevelManager& LevelManager::getInstance() {
    static LevelManager instance;
    return instance;
}

void LevelManager::loadAllLevels(const std::string& pathToFile) {
    std::ifstream file(pathToFile);
    if (!file) {
        spdlog::error("Could not open file: {}", pathToFile);
        failedPaths.push_back(pathToFile);
        return;
    }

    std::string line;
    int levelId = 0;
    while (std::getline(file, line)) {
        spdlog::info("Loading level from path: {}", line);
        auto level = std::make_shared<Level>();
        level->levelId = levelId++;
        level->loadModel(line);
        level->_transform._position = defaultPosition;
        level->calculateHitboxes();

        std::ifstream checkFile(line);
        if (!checkFile) {
            spdlog::error("Failed to load level from path: {}", line);
            failedPaths.push_back(line);
        } else {
            levels.push_back(level);
            successfulPaths.push_back(line);
            levelCount++;
            spdlog::info("Successfully loaded level {}. Total levels loaded: {}", line, levelCount);
        }
    }

    loadLocomotive();
    loadInvisibleWalls();
}

void LevelManager::loadLocomotive() {
    spdlog::info("loading locomotive");
    locomotive = std::make_shared<Object3D>();
    locomotive->loadModel("../../res/models/Assets/loco/lokomotywa.obj"); // TODO: make sure this path is valid
    locomotive->_transform._position = {-7.039, 4.693, 4.022};
    //    locomotive->_transform._position = defaultPosition;
//    locomotive->_transform._position.x = levels.at(levels.size()-1)->modelMaxVertex.x;
}

void LevelManager::loadInvisibleWalls() {
    spdlog::info("loading invisible walls");
    Model allWalls("../../res/models/Levels/additionalHitboxes.obj");
    for (auto& mesh: allWalls.meshes) {
        auto hitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
        hitbox->Create(levels.at(0).get()); // jezu ale to obrzydliwe
        hitbox->calculateFromMesh(mesh);
        hitboxes.push_back(hitbox);
        spdlog::info("Invisible wall from mesh {}, min = ({},{},{}), max = ({}, {},{})", mesh._name,
            hitbox->currentMin().x, hitbox->currentMin().y, hitbox->currentMin().z,
            hitbox->currentMax().x, hitbox->currentMax().y, hitbox->currentMax().z
        );
    }
}

void LevelManager::loadAllLevelsData(const std::string& pathToFile) {
    std::ifstream file(pathToFile);
    if (!file) {
        spdlog::error("Could not open file: {}", pathToFile);
        failedPaths.push_back(pathToFile);
        return;
    }

    std::string line;
    int i = 0;
    while (std::getline(file, line)) {
        spdlog::info("Loading level data from path: {}", line);
        levels[i]->LoadDataFromJson(Engine::parser.CreateFromJSONLevelData(line));
        i++;
    }
}

std::vector<LevelManager::LevelPtr>& LevelManager::getLevels() {
    return levels;
}

void LevelManager::clearLevels() {
    levels.clear();
}

std::shared_ptr<Level> LevelManager::getCurrentLevel() {
    if (!levels.empty() && currentLevel < levels.size()) {
        return levels[currentLevel];
    }
    spdlog::error("Cannot get current level - no levels loaded or index out of range");
    return nullptr;
}


bool LevelManager::nextLevel() {
    if (currentLevel < levels.size() - 1) { // Check if there are more levels
        currentLevel++;
        spdlog::info("Advanced to level {}. Total levels: {}", currentLevel, levelCount);
        return true;
    } else {
        spdlog::info("No more levels. You have completed the game!");
        return false;
    }
}

LevelManager::LevelManager() {
}

LevelManager::~LevelManager() {
    clearLevels();
}

void LevelManager::setCurrentLevel(int currentLevel) {
    LevelManager::currentLevel = currentLevel;
}



#include "../include/LevelManager.h"

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
        level->_transform._position.x = -7.039000034332275;
        level->_transform._position.y = 4.692999839782715;
        level->_transform._position.z = 4.35699987411499;
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

void LevelManager::ImGui() {
    ImGui::Begin(getWindowName().c_str());

    ImGui::SetWindowSize(ImVec2(300, 500));

    if (ImGui::TreeNode("Loaded Levels")) {
        for (const auto& path : successfulPaths) {
            ImGui::Text("%s", path.c_str());
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Failed Levels")) {
        for (const auto& path : failedPaths) {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", path.c_str());
        }
        ImGui::TreePop();
    }

    if (!levels.empty()) {
        ImGui::SliderInt("Current Level", &currentLevel, 0, levels.size() - 1);
    }

    for(auto& level: levels) {
        level->HideImgui();
    }
    getCurrentLevel()->ShowImgui();

    ImGui::End();
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
    setWindowName("Level Manager");
}

LevelManager::~LevelManager() {
    clearLevels();
}

#include "../include/Objects/Level.h"
#include "../include/Objects/Hitbox.h"
#include "spdlog/spdlog.h"
#include "../../include/Engine.h"

Level::Level() {
    IGui::setWindowName("Level");
}

Level::~Level() {
    spdlog::error("Level destructor");
    hitboxes.clear();
}

void Level::loadLevel(const std::string& fileName) {
    loadFromJSON(Engine::parser.CreateFromJSONWalls(fileName));
    calculateHitboxes();
    assignTargetsToPlatforms();
    assignButtonsToPlatforms();
}

void Level::calculateHitboxes() {
    for (const Mesh& mesh : _model->meshes) {
        std::shared_ptr<Hitbox> hitbox = nullptr;
        char type = mesh._name[0];

        // Extract the ID from the mesh name
        std::string idString = mesh._name.substr(2, mesh._name.find('_', 2) - 2);  // extract from third character to the next '_'
        int id;
        try {
            id = std::stoi(idString);  // try to convert to integer
        } catch (const std::invalid_argument& e) {
            id = -1;  // if conversion fails, assign -1 as default value
        }

        switch(type) {
            case 'D': { // Door
                hitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                hitbox->tag = "door";
                break;
            }

            case 'F': { // Floor
                hitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                hitbox->tag = "floor";
                break;
            }

            case 'W': { // Wall
                hitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                hitbox->tag = "wall";
                break;
            }

            case 'R': { // Roof
                hitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                hitbox->tag = "roof";
                break;
            }

            case 'B': { // Button
                auto buttonHitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                buttonHitbox->tag = "button";

                auto button = std::make_shared<Button>(nullptr, glm::vec3(0));
                buttonHitbox->Create(button);
                button->id = id;
                buttons.push_back(button);

                spdlog::info("Button created");
                break;
            }

            case 'C': { // Decoration around button
                break;
            }

            case 'S': { // Static platform
                hitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                hitbox->tag = "static platform";
                break;
            }

            case 'M': { // Moving platform
                hitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                hitbox->tag = "moving platform";
                // TODO: get actual position for origin
                auto platform = std::make_shared<Platform>(glm::vec3(0), glm::vec3(0), 0);
                platform->id = id;
                movingPlatforms.push_back(platform);

                spdlog::info("Moving platform created");
                break;
            }

            case 'T': { // Target of moving platform
                // TODO: get actual position for targer
                glm::vec3 position = mesh.vertices.at(0).Position;
                targetPositions[id] = position;

                break;
            }

            default: { // Default behavior
                hitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                spdlog::warn("unrecognized mesh name: {}", mesh._name);

                break;
            }
        }
        if(hitbox != nullptr) {
            hitbox->calculateFromMesh(mesh);
            hitbox->Create(Object3D::shared_from_this());
            hitboxes.push_back(hitbox);

            spdlog::info("Calculated hitbox for {}", mesh._name);
        } else {
            spdlog::info("No hitbox created for {}", mesh._name);
        }

    }
}

void Level::assignTargetsToPlatforms() {
    for (auto &platform : movingPlatforms) {
        auto found = targetPositions.find(platform->id);
        if (found != targetPositions.end()) {
            platform->positionTarget = found->second;
        }
        else {
            spdlog::warn("No target position found for platform with ID: {}", platform->id);
        }
    }
}

void Level::assignButtonsToPlatforms() {
    for (auto &button : buttons) {
        auto platform = std::ranges::find_if(movingPlatforms,
                                             [&button](const std::shared_ptr<Platform>& platform) {
                                                 return platform->id == button->id;
                                             });


        if (platform != movingPlatforms.end()) {
            button->connectedPlatform = platform->get();
        }
        else {
            spdlog::warn("No matching platform found for button with ID: {}", button->id);
        }
    }
}

void Level::logHitboxes() {
    spdlog::warn("{} hitboxes loaded", hitboxes.size());
    for (const auto& hitbox : hitboxes) {
        spdlog::info("({}, {}) to ({}, {})", hitbox->_min.x, hitbox->_min.y, hitbox->_max.x, hitbox->_max.y);
    }
}

void Level::ImGui() {
    ImGui::Begin(getWindowName().c_str());
    Object3D::ImGui();

    ImGui::SetWindowSize(ImVec2(300, 700));

    int x = 0;
    ImGui::SliderInt("visible hitbox", &x, 0, hitboxes.size() - 1);
    for (const auto& hitbox : hitboxes) {
        hitbox->draw = false;
    }
    hitboxes.at(x)->draw = true;

    ImGui::Checkbox("Show hitboxes", &areRendered);
    if (areRendered) {
        //spdlog::warn("showing hitboxes");
        for (const auto& hitbox : hitboxes) {
            hitbox->draw = true;
        }
    }

    if (ImGui::Button("SAVE WALLS")) {
        Engine::parser.SaveJSON(this->ParseToJSON(), "objects/walls");
    }

    ImGui::End();
}

void Level::onCollision(std::shared_ptr<Object3D> other) {
    // Implement collision handling logic for walls
}

rapidjson::Document Level::ParseToJSON() {
    rapidjson::Document d;
    d.SetObject();
    d.AddMember("type", "object3D", d.GetAllocator());
    d.AddMember("scaleX", _transform._scale.x, d.GetAllocator());
    d.AddMember("scaleY", _transform._scale.y, d.GetAllocator());
    d.AddMember("scaleZ", _transform._scale.z, d.GetAllocator());
    d.AddMember("rotationX", _transform._rotation.x, d.GetAllocator());
    d.AddMember("rotationY", _transform._rotation.y, d.GetAllocator());
    d.AddMember("rotationZ", _transform._rotation.z, d.GetAllocator());
    d.AddMember("positionX", _transform._position.x, d.GetAllocator());
    d.AddMember("positionY", _transform._position.y, d.GetAllocator());
    d.AddMember("positionZ", _transform._position.z, d.GetAllocator());
    rapidjson::Value n(_path.c_str(), d.GetAllocator());
    d.AddMember("model", n, d.GetAllocator());

    return d;
}

void Level::loadFromJSON(const Level& temp) {
    this->loadModel(temp._path);
    this->_transform._scale = temp._transform._scale;
    this->_transform._rotation = temp._transform._rotation;
    this->_transform._position = temp._transform._position;
    this->calculateHitboxes();
}

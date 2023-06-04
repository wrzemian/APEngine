#include "../include/Objects/Level.h"
#include "spdlog/spdlog.h"
#include "../../include/Engine.h"
#include "../../include/Objects/Box.h"

Level::Level() {
    IGui::setWindowName("Level");
}

Level::~Level() {
    spdlog::error("Level destructor");
    hitboxes.clear();
}

[[nodiscard]] int getButtonId(const std::string& name) {
    // Extract the ID from the mesh name
    std::string idString = name.substr(2, name.find('_', 2) - 2);  // extract from third character to the next '_'
    int id = 0;
    try {
        id = std::stoi(idString);  // try to convert to integer
    } catch (const std::invalid_argument& e) {
        id = 0;  // if conversion fails, assign -1 as default value
    }
    return id;
}

[[nodiscard]] int getTargetId(const std::string& name) {
    int targetId = 0;
    std::string targetIdString;
    if (!name.empty()) {
        char lastChar = name.back();
        if (std::isdigit(lastChar)) {
            targetIdString = std::string(1, lastChar);
            try {
                targetId = std::stoi(targetIdString);
            } catch (const std::invalid_argument& e) {
                targetId = 0;  // Handle the case where the conversion to integer fails
            }
        }
    }
    return targetId;
}

void Level::calculateHitboxes() {
    spdlog::info("CALCULATE HITBOXES for {}", _model->directory);

    for (const Mesh& mesh : _model->meshes) {
        spdlog::warn("MESH NAME = {}", mesh._name);
    }

    for (const Mesh& mesh : _model->meshes) {
        std::shared_ptr<Hitbox> hitbox = nullptr;
        char type = mesh._name[0];

        int buttonId = getButtonId(mesh._name);
        int targetId = getTargetId(mesh._name);

        int totalId = 100 * targetId + buttonId;

        switch(type) {
            case 'D': { // Door
                if(winArea == nullptr) {
                    spdlog::info("created door object");
                    winArea = std::make_shared<WinArea>();
                    winArea->setShader(_shader);
                    winArea->tag = "winArea";
                    winArea->ShowImgui();
                    winArea->_transform._position = _transform._position;
                    winArea->_model = std::make_shared<Model>();
                }
                winArea->_model->meshes.push_back(mesh); // Add the current mesh to the button's model
                auto doorHitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                doorHitbox->tag = "winArea";
                doorHitbox->calculateFromMesh(mesh);
                doorHitbox->Create(winArea.get());
                doorHitbox->isTrigger = true;

                hitboxes.push_back(doorHitbox);

                //staticModel.meshes.push_back(mesh);
                spdlog::info("Door created {}", mesh._name);

                break;
            }

            case 'F': { // Floor
                hitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                hitbox->calculateFromMesh(mesh);
                hitbox->Create(this);

                staticModel.meshes.push_back(mesh);
                hitbox->tag = "floor";

                spdlog::info("Floor created {}", mesh._name);

                break;
            }

            case 'W': { // Wall
                hitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                hitbox->calculateFromMesh(mesh);
                hitbox->Create(this);

                staticModel.meshes.push_back(mesh);
                hitbox->tag = "wall";

                spdlog::info("Wall created {}", mesh._name);

                break;
            }

            case 'Y': { // Wall
                hitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                hitbox->calculateFromMesh(mesh);
                hitbox->Create(this);

                staticModel.meshes.push_back(mesh);
                hitbox->tag = "wall";

                spdlog::info("Wall (pręt) created {}", mesh._name);

                break;
            }

            case 'R': { // Roof
                hitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                hitbox->calculateFromMesh(mesh);
                hitbox->Create(this);

                staticModel.meshes.push_back(mesh);
                hitbox->tag = "roof";

                spdlog::info("Roof created {}", mesh._name);

                break;
            }

            case 'B': { // Button
                auto buttonHitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                buttonHitbox->tag = "button";
                hitboxes.push_back(buttonHitbox);

                auto button = std::make_shared<Button>(nullptr, glm::vec3(0));
                button->setShader(_shader);
                button->tag = "button";
                button->ShowImgui();
                button->_transform._position = _transform._position;

                button->_model = std::make_shared<Model>();
                button->_model->meshes.push_back(mesh); // Add the current mesh to the button's model
                buttonHitbox->Create(button.get());
                button->id = totalId;
                buttons.push_back(button);

                buttonHitbox->calculateFromMesh(mesh);
                buttonHitbox->Create(button.get());
                buttonHitbox->isTrigger = true;

                spdlog::info("Button created {}", mesh._name);
                break;
            }

            case 'C': { // Decoration around button
                spdlog::info("Decoration around button {}", mesh._name);

                staticModel.meshes.push_back(mesh);
                break;
            }

            case 'X': { // Decoration
                spdlog::info("Other decoration {}", mesh._name);

                staticModel.meshes.push_back(mesh);
                break;
            }

            case 'S': { // Static platform
                hitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                hitbox->calculateFromMesh(mesh);
                hitbox->Create(this);

                staticModel.meshes.push_back(mesh);

                hitbox->tag = "static platform";

                spdlog::info("Platform created {}", mesh._name);

                break;
            }

            case 'M': { // Moving platform
                auto platformHitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                platformHitbox->tag = "moving platform";
                hitboxes.push_back(platformHitbox);

                spdlog::info("Moving platform position {}, {}, {}", _transform._position.x,_transform._position.y,_transform._position.z);

                auto platform = std::make_shared<Platform>(_transform._position, glm::vec3(0), 0);
                platform->tag = "moving platform";
                platform->ShowImgui();
                platform->id = totalId;

                platform->_transform._position = _transform._position;
                platform->setShader(_shader);
                platform->_model = std::make_shared<Model>();
                platform->_model->meshes.push_back(mesh); // Add the current mesh to the platform's model
                platform-> speed = 1;
                movingPlatforms.push_back(platform);

                platformHitbox->calculateFromMesh(mesh);
                platformHitbox->Create(platform.get());

                spdlog::info("Moving platform created {}", mesh._name);
                break;
            }

            case 'T': { // Target of moving platform
                Hitbox test(Hitbox::STATIC);
                test.Create(this);
                test.calculateFromMesh(mesh);
                auto middle = (test._min + test._max) * 0.5f;

                spdlog::info("Target position {} = {}, {}, {}", mesh._name, middle.x, middle.y, middle.z);

                targetPositions[totalId] = middle;

                break;
            }

            case 'E': { // Box
                // TODO: change to Box implementation
                auto box = std::make_shared<Box>();
                boxes.push_back(box);
                box->tag = "box";

                auto test = std::make_shared<Hitbox>(Hitbox::DYNAMIC);
                test->calculateFromMesh(mesh);

                test->Create(box.get());
                test->draw = true;
                hitboxes.push_back(test);
                test->tag = "box";

                box->_transform._position = _transform._position;
                box->setShader(_shader);
                box->_model = std::make_shared<Model>();
                box->_model->meshes.push_back(mesh); // Add the current mesh to the platform's model
                box->_transform._position = _transform._position;

                spdlog::info("Box created from {}", mesh._name);

                break;
            }

            case 'P': { // Battery
                auto battery = std::make_shared<Battery>();
                battery->tag = "battery";
                batteries.push_back(battery);

                battery->_transform._position = _transform._position;
                battery->setShader(_shader);
                battery->_model = std::make_shared<Model>();
                battery->_model->meshes.push_back(mesh); // Add the current mesh to the platform's model

                auto test = std::make_shared<Hitbox>(Hitbox::DYNAMIC);
                test->tag = "battery";
                test->calculateFromMesh(mesh);
                test->Create(battery.get());
                hitboxes.push_back(test);
                test->draw = true;

                spdlog::info("Battery created from {}", mesh._name);

                break;
            }

            default: { // Default behavior
                hitbox = std::make_shared<Hitbox>(Hitbox::DYNAMIC);
                hitbox->calculateFromMesh(mesh);
                hitbox->Create(this);
                spdlog::warn("unrecognized mesh name: {}", mesh._name);

                break;
            }
        }
        if(hitbox != nullptr) {
            hitboxes.push_back(hitbox);
        }
    }
    assignTargetsAndPlatforms();
}

void Level::assignTargetsAndPlatforms() {
    for (const auto& button : buttons) {
        for (const auto& platform : movingPlatforms) {
            spdlog::info("Checking buttonId {} with platformId {} ({})", button->id, platform->id, platform->id % 100 == button->id % 100);
            if (platform->id % 100 == button->id % 100) {
                button->addPlatform(platform.get());
                spdlog::info("Button {} connected to Platform {}", button->id, platform->id);
            }
        }
        if (button->connectedPlatforms.empty()) {
            spdlog::warn("Button {} does not have a matching platform.", button->id);
        }
    }

    for (const auto& platform : movingPlatforms) {
        auto it = targetPositions.find(platform->id);
        if (it != targetPositions.end()) {
            //TODO: reconsider with graphic designers
            Hitbox test(Hitbox::STATIC);
            test.Create(platform.get());
            test.calculateFromModel(*platform->_model);
            auto middle = (test._min + test._max) * 0.5f;
            spdlog::info("Platform id {} in the middle in ({}, {}, {}) has target ({}, {}, {})", platform->id, middle.x, middle.y, middle.z, it->second.x, it->second.y, it->second.z);
            platform->positionTarget = it->second - middle + _transform._position;
            spdlog::info("assigned target position: ({}, {}, {})", platform->positionTarget.x, platform->positionTarget.y, platform->positionTarget.z);
        } else {
            spdlog::warn("No target position found for platform {}", platform->id);
        }
    }
}




void Level::logNewObjects() {
    spdlog::info("LOADED OBJECTS:");

    // Logging hitboxes
    spdlog::info("Hitboxes:");
    for (const auto& hitbox : hitboxes) {
        hitbox->logFields();
    }

    // Logging platforms
    spdlog::info("Platforms:");
    for (const auto& platform : movingPlatforms) {
        platform->logFields();
    }

    // Logging buttons
    spdlog::info("Buttons:");
    for (const auto& button : buttons) {
        button->logFields();
    }

    // Logging boxes
    spdlog::info("Boxes:");
    for (const auto& box : boxes) {
        //box->logFields(); TODO: add logging
        spdlog::info("box at {}, {}, {}", box->_transform._position.x, box->_transform._position.y, box->_transform._position.z);
    }

    // Logging batteries
    spdlog::info("Batteries:");
    for (const auto& battery : batteries) {
        //battery->logFields(); TODO: ad logging
        spdlog::info("battery at {}, {}, {}", battery->_transform._position.x, battery->_transform._position.y, battery->_transform._position.z);

    }

    // Log target positions
    spdlog::info("Target Positions:");
    for (const auto& pair : targetPositions) {
        spdlog::info("- id: {}, position: ({}, {}, {})", pair.first, pair.second.x, pair.second.y, pair.second.z);
    }

    spdlog::info("");
}


void Level::logHitboxes() {
    spdlog::warn("{} hitboxes loaded", hitboxes.size());
    for (const auto& hitbox : hitboxes) {
        spdlog::info("Hitbox ({}, {}) to ({}, {})", hitbox->_min.x, hitbox->_min.y, hitbox->_max.x, hitbox->_max.y);
    }
}

void Level::ImGui() {
    ImGui::Begin(getWindowName().c_str());
    Object3D::ImGui();

    ImGui::SetWindowSize(ImVec2(300, 500));

    ImGui::Checkbox("Buttons imgui", &buttonsImgui);
    for (const auto& button : buttons) {
        button->setImgui(buttonsImgui);
    }

    ImGui::Checkbox("Platforms imgui", &platformsImgui);
    for (const auto& platform : movingPlatforms) {
        platform->setImgui(platformsImgui);
    }

    ImGui::Checkbox("Boxes imgui", &boxesImgui);
    for (const auto& box : boxes) {
        box->setImgui(boxesImgui);
    }

    ImGui::Checkbox("Batteries imgui", &batteriesImgui);
    for (const auto& battery : batteries) {
        battery->setImgui(batteriesImgui);
    }

    if (ImGui::Button("SAVE WALLS")) {
        Engine::parser.SaveJSON(this->ParseToJSON(), "objects/walls");
    }

    ImGui::End();
}

void Level::onCollision(Object3D* other) {
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

void Level::setShader(Shader* shader) {
    _shader = shader;

    for (const auto& platform : movingPlatforms) {
        platform->setShader(shader);
    }

    for (const auto& button : buttons) {
        button->setShader(shader);
    }
};

void Level::Draw() {
    //shader.use();
    if(_model == nullptr) {
        spdlog::error("null model in {}", tag);
        return;
    }
    if(_shader == nullptr) {
        spdlog::error("null shader in {}", tag);
        return;
    }
    _transform.updateWorldTransform(glm::mat4(1.0f), *_shader);
    // shader.setMat4("model", _transform.getModel());

    staticModel.Draw(*_shader);
}
#include "../include/Objects/Walls.h"
#include "../include/Objects/Hitbox.h"
#include "spdlog/spdlog.h"
#include "../../include/Engine.h"
#include "../include/Objects/Button.h"

Walls::Walls() {
    IGui::setWindowName("Walls");
}

Walls::~Walls() {
    spdlog::error("Walls destructor");
    hitboxes.clear();
}

void Walls::calculateHitboxes() {
    spdlog::info("CALCULATE HITBOXES for {}", _model->directory);

    for (const Mesh& mesh : _model->meshes) {
        spdlog::warn("MESH NAME = {}", mesh._name);
    }

    for (const Mesh& mesh : _model->meshes) {
        std::shared_ptr<Hitbox> hitbox = nullptr;
        char type = mesh._name[0];

        // Extract the ID from the mesh name
        std::string idString = mesh._name.substr(2, mesh._name.find('_', 2) - 2);  // extract from third character to the next '_'
        int id;
        try {
            id = std::stoi(idString);  // try to convert to integer
        } catch (const std::invalid_argument& e) {
            id = 0;  // if conversion fails, assign -1 as default value
        }
        spdlog::info("id1 = {}", idString);

        int targetId;
        std::string targetIdString;
        if (!mesh._name.empty()) {
            char lastChar = mesh._name.back();
            if (std::isdigit(lastChar)) {
                targetIdString = std::string(1, lastChar);
                try {
                    targetId = std::stoi(targetIdString);
                } catch (const std::exception& e) {
                    targetId = 0;  // Handle the case where the conversion to integer fails
                }
            }
        }
        spdlog::info("id2 = {}", targetId);

        int totalId = 100 * targetId + id;

        switch(type) {
            case 'D': { // Door
                hitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                hitbox->calculateFromMesh(mesh);
                hitbox->Create(this);

                //TODO: implement class Door
                staticModel.meshes.push_back(mesh);
                hitbox->tag = "door";

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
                //TODO: figure out what to do with "pręt"s
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

                // TODO: get actual position for origin
                spdlog::info("Moving platform position {}, {}, {}", _transform._position.x,_transform._position.y,_transform._position.z);

                auto platform = std::make_shared<Platform>(_transform._position, glm::vec3(0), 0);
                platform->tag = "moving platform";
                platform->ShowImgui();
                platform->_transform._position = _transform._position;
                platform->setShader(_shader);

                platform->id = totalId;

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
                // TODO: get actual position for targer

                glm::vec3 position = mesh.vertices.at(0).Position;
                spdlog::info("Target position {} = {}, {}, {}", mesh._name, position.x, position.y, position.z);

                targetPositions[totalId] = position;

                break;
            }

            default: { // Default behavior
                hitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                hitbox->calculateFromMesh(mesh);
                hitbox->Create(this);
                spdlog::warn("unrecognized mesh name: {}", mesh._name);

                break;
            }
        }
        if(hitbox != nullptr) {
            hitboxes.push_back(hitbox);

            spdlog::info("Calculated hitbox for {}", mesh._name);
        } else {
            spdlog::info("No hitbox created for {}", mesh._name);
        }

    }
    assignTargetsAndPlatforms();
    //logNewObjects();
}

void Walls::assignTargetsAndPlatforms() {
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
        spdlog::info("Looking for target position for platformId {}", platform->id);
        auto it = targetPositions.find(platform->id);
        if (it != targetPositions.end()) {
            platform->positionTarget = it->second;
            spdlog::info("Platform {} assigned target position: ({}, {}, {})", platform->id, platform->positionTarget.x, platform->positionTarget.y, platform->positionTarget.z);
        } else {
            spdlog::warn("No target position found for platform {}", platform->id);
        }
    }
}




void Walls::logNewObjects() {
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

    // Log target positions
    spdlog::info("Target Positions:");
    for (const auto& pair : targetPositions) {
        spdlog::info("- id: {}, position: ({}, {}, {})", pair.first, pair.second.x, pair.second.y, pair.second.z);
    }

    spdlog::info("");
}


void Walls::logHitboxes() {
    spdlog::warn("{} hitboxes loaded", hitboxes.size());
    for (const auto& hitbox : hitboxes) {
        spdlog::info("({}, {}) to ({}, {})", hitbox->_min.x, hitbox->_min.y, hitbox->_max.x, hitbox->_max.y);
    }
}

void Walls::ImGui() {
    ImGui::Begin(getWindowName().c_str());
    Object3D::ImGui();

    ImGui::SetWindowSize(ImVec2(300, 400));

    ImGui::Checkbox("Buttons imgui", &buttonsImgui);
    for (const auto& button : buttons) {
        button->setImgui(buttonsImgui);
    }

    ImGui::Checkbox("Platforms imgui", &platformsImgui);
    for (const auto& platform : movingPlatforms) {
        platform->setImgui(platformsImgui);
    }

    if (ImGui::Button("SAVE WALLS")) {
        Engine::parser.SaveJSON(this->ParseToJSON(), "objects/walls");
    }

    ImGui::End();
}

void Walls::onCollision(Object3D* other) {
    // Implement collision handling logic for walls
}

rapidjson::Document Walls::ParseToJSON() {
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

void Walls::loadFromJSON(const Walls& temp) {
    this->loadModel(temp._path);
    this->_transform._scale = temp._transform._scale;
    this->_transform._rotation = temp._transform._rotation;
    this->_transform._position = temp._transform._position;
    this->calculateHitboxes();
}

void Walls::setShader(Shader* shader) {
    _shader = shader;

    for (const auto& platform : movingPlatforms) {
        platform->setShader(shader);
    }

    for (const auto& button : buttons) {
        button->setShader(shader);
    }
};

void Walls::Draw() {
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

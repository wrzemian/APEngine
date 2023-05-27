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
                //TODO: implement class Door
                staticModel.meshes.push_back(mesh);
                hitbox->tag = "door";
                break;
            }

            case 'F': { // Floor
                hitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                staticModel.meshes.push_back(mesh);
                hitbox->tag = "floor";
                break;
            }

            case 'W': { // Wall
                hitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                staticModel.meshes.push_back(mesh);
                hitbox->tag = "wall";
                break;
            }

            case 'R': { // Roof
                hitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                staticModel.meshes.push_back(mesh);
                hitbox->tag = "roof";
                break;
            }

            case 'B': { // Button
                auto buttonHitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                buttonHitbox->tag = "button";

                auto button = std::make_shared<Button>(nullptr, glm::vec3(0));
                button->setShader(_shader);
                button->tag = "button";

                button->_model = std::make_shared<Model>();
                button->_model->meshes.push_back(mesh); // Add the current mesh to the button's model
                buttonHitbox->Create(button.get());
                button->id = id;
                buttons.push_back(button);

                spdlog::info("Button created {}", mesh._name);
                break;
            }

            case 'C': { // Decoration around button
                staticModel.meshes.push_back(mesh);
                break;
            }

            case 'S': { // Static platform
                hitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                staticModel.meshes.push_back(mesh);

                hitbox->tag = "static platform";
                break;
            }

            case 'M': { // Moving platform
                hitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                hitbox->tag = "moving platform";

                // TODO: get actual position for origin
                glm::vec3 position = mesh.vertices.at(0).Position;
                auto platform = std::make_shared<Platform>(position, glm::vec3(0), 0);
                platform->tag = "moving platform";

                platform->setShader(_shader);

                platform->id = id;
                platform->_model = std::make_shared<Model>();
                platform->_model->meshes.push_back(mesh); // Add the current mesh to the platform's model
                movingPlatforms.push_back(platform);

                spdlog::info("Moving platform created {}", mesh._name);
                break;
            }

            case 'T': { // Target of moving platform
                // TODO: get actual position for targer
                spdlog::info("Target position {}", mesh._name);

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
            hitbox->Create(this);
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
        const int buttonId = button->id;
        auto it = targetPositions.find(buttonId);
        if (it != targetPositions.end()) {
            const glm::vec3& targetPosition = it->second;
            button->connectedPlatform = nullptr;
            for (const auto& platform : movingPlatforms) {
                if (platform->id == buttonId) {
                    platform->positionTarget = targetPosition;
                    button->connectedPlatform = platform.get();
                    //break;
                }
            }
            if (button->connectedPlatform) {
                spdlog::info("Button {} connected to Platform {}", buttonId, button->connectedPlatform->id);
            } else {
                spdlog::warn("Button {} does not have a matching platform.", buttonId);
            }
        } else {
            spdlog::warn("Button {} does not have a matching target position.", buttonId);
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
    spdlog::error("Walls draw", tag);

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

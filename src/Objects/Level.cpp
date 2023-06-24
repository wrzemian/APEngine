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
        spdlog::warn("MESH NAME = {}, vertices count = {}", mesh._name, mesh.vertices.size());
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
                    winArea->tag = "winArea";
                    //winArea->ShowImgui();
                    winArea->_transform._position = _transform._position;
                    winArea->_model = std::make_shared<Model>();
                    winArea->_path = mesh._name;

                }
                winArea->_model->meshes.push_back(mesh); // Add the current mesh to the button's model
                winArea->calculateBoundingBox();

                auto areaTriggerHitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                areaTriggerHitbox->calculateFromMesh(mesh);
                areaTriggerHitbox->Create(winArea.get());
                areaTriggerHitbox->tag = "winArea";
                areaTriggerHitbox->_offset.x = -0.1f;
                areaTriggerHitbox->_offset.y = 0.0f;
                areaTriggerHitbox->_offset.z = 0.0f;
                areaTriggerHitbox->_color.x = 0.5;
                areaTriggerHitbox->_color.y = 1.0;
                areaTriggerHitbox->_color.z = 1.0;
                areaTriggerHitbox->_max.y  -= 0.5f;
                areaTriggerHitbox->_min.x  -= 1.2f;
                areaTriggerHitbox->isTrigger = true;
                hitboxes.push_back(areaTriggerHitbox);

                auto doorHitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                doorHitbox->tag = "winArea";
                doorHitbox->calculateFromMesh(mesh);
                doorHitbox->Create(winArea.get());
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
                auto wall = std::make_shared<Object3D>(); // Create a new wall object
                wall->_transform._position = _transform._position; // Set the wall's position
                wall->_model = std::make_shared<Model>(); // Create a new model for the wall
                wall->_model->meshes.push_back(mesh); // Add the current mesh to the wall's model
                wall->tag = "wall"; // Tag the wall
                wall->_path = mesh._name;

                hitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                hitbox->calculateFromMesh(mesh);
                hitbox->Create(wall.get());

                // Add the wall object to a suitable collection in your level
                walls.push_back(wall);


                staticModel.meshes.push_back(mesh);
                hitbox->tag = "wall";

                spdlog::info("Wall created {}", mesh._name);

                break;
            }

            case 'Y': { // Wall
                auto wall = std::make_shared<Object3D>(); // Create a new wall object
                wall->_transform._position = _transform._position; // Set the wall's position
                wall->_model = std::make_shared<Model>(); // Create a new model for the wall
                wall->_model->meshes.push_back(mesh); // Add the current mesh to the wall's model
                wall->tag = "wall"; // Tag the wall
                wall->_path = mesh._name;

                hitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                hitbox->calculateFromMesh(mesh);
                hitbox->Create(wall.get());

                // Add the wall object to a suitable collection in your level
                walls.push_back(wall);


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
                button->tag = "button";
               // button->ShowImgui();
                button->_transform._position = _transform._position;

                button->_model = std::make_shared<Model>();
                button->_model->meshes.push_back(mesh); // Add the current mesh to the button's model
                button->calculateBoundingBox();
                button->_path = mesh._name;

                //buttonHitbox->Create(button.get());
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
                platform->levelId = levelId;
                platform->_path = mesh._name;

                platform->tag = "moving platform";
              //  platform->ShowImgui();
                platform->id = totalId;

                platform->_transform._position = _transform._position;
                platform->_model = std::make_shared<Model>();
                platform->_model->meshes.push_back(mesh); // Add the current mesh to the platform's model
                platform->calculateBoundingBox();

                platform-> speed = 1;
                movingPlatforms.push_back(platform);

                platformHitbox->calculateFromMesh(mesh);
                platformHitbox->Create(platform.get());

                spdlog::info("Moving platform created {}", mesh._name);
                break;
            }

            case 'T': { // Target of moving platform
                auto middle = (mesh.vertices.at(0).Position + mesh.vertices.at(0).Position) * 0.5f;

                spdlog::info("Target position {} = {}, {}, {}", mesh._name, middle.x, middle.y, middle.z);

                targetPositions[totalId] = middle;

                break;
            }

            case 'E': { // Box
                // TODO: change to Box implementation
                auto box = std::make_shared<Box>();
                box->StopMoving();
                //box->ShowImgui();
                box->levelId = levelId;
                boxes.push_back(box);
                box->tag = "box";
                box->_path = mesh._name;

                box->_transform._position = _transform._position;
                box->_model = std::make_shared<Model>();
                box->_model->meshes.push_back(mesh); // Add the current mesh to the platform's model

                auto middle = box->modelMiddle;
                spdlog::warn("box middle at {}, {}, {}", middle.x, middle.y, middle.z);
                spdlog::warn("box vertices: {}", box->_model->meshes.at(0).vertices.size());

                box->changeVertexPositions(-middle);
                box->calculateBoundingBox();

                box->_transform._position = _transform._position;
                box->_transform._position += middle;

                auto boxHitbox = std::make_shared<Hitbox>(Hitbox::DYNAMIC);
                boxHitbox->calculateFromModel(*box->_model);

                boxHitbox->Create(box.get());
                hitboxes.push_back(boxHitbox);
                boxHitbox->tag = "box";

                spdlog::info("Box created from {}", mesh._name);

                break;
            }

            case 'P': { // Battery
                auto battery = std::make_shared<Battery>();
                battery->StopMoving();
                battery->_transform._position.y += 0.1f;
                //battery->ShowImgui();
                battery->levelId = levelId;
                battery->_path = mesh._name;

                battery->tag = "battery";
                batteries.push_back(battery);

                battery->_transform._position = _transform._position;
                battery->_model = std::make_shared<Model>();
                battery->_model->meshes.push_back(mesh); // Add the current mesh to the platform's model
                battery->calculateBoundingBox();

                auto test = std::make_shared<Hitbox>(Hitbox::DYNAMIC);
                test->tag = "battery";
                test->calculateFromMesh(mesh);
                test->Create(battery.get());
                hitboxes.push_back(test);
                test->draw = true;



                spdlog::info("Battery created from {}", mesh._name);

                break;
            }

            case 'K': { // door doorButton
                auto buttonHitbox = std::make_shared<Hitbox>(Hitbox::STATIC);
                buttonHitbox->tag = "button";
                hitboxes.push_back(buttonHitbox);

                doorButton = std::make_shared<Button>(nullptr, glm::vec3(0));
                doorButton->tag = "button";
                doorButton->_path = mesh._name;
                doorButton->id = totalId;

                // doorButton->ShowImgui();
                doorButton->_transform._position = _transform._position;

                doorButton->_model = std::make_shared<Model>();
                doorButton->_model->meshes.push_back(mesh); // Add the current mesh to the doorButton's model
                doorButton->calculateBoundingBox();

                buttonHitbox->calculateFromMesh(mesh);
                buttonHitbox->Create(doorButton.get());
                buttonHitbox->isTrigger = true;

                spdlog::info("Door button created {}", mesh._name);
                break;
            }

            case 'J': { // Cable
                auto cable = std::make_shared<Cable>();
                cable->tag = "cable";
                cable->_path = mesh._name;
                // cable->ShowImgui();
                cable->_transform._position = _transform._position;

                cable->_model = std::make_shared<Model>();
                cable->_model->meshes.push_back(mesh); // Add the current mesh to the cable's model
                cable->calculateBoundingBox();

                //buttonHitbox->Create(cable.get());
                cable->id = totalId;
                cables.push_back(cable);

                spdlog::info("Cable created {}", mesh._name);
                break;
            }

            default: { // Default behavior
                spdlog::warn("unrecognized mesh name: {}", mesh._name);

//                hitbox = std::make_shared<Hitbox>(Hitbox::DYNAMIC);
//                hitbox->calculateFromMesh(mesh);
//                hitbox->Create(this);

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
    spdlog::info("Wagon min({}, {}, {}), max({}, {}, {}) size({}, {}, {})",
                 modelMinVertex.x, modelMinVertex.y, modelMinVertex.z,
                 modelMaxVertex.x, modelMaxVertex.y, modelMaxVertex.z,
                 size.x, size.y, size.z);
    for (const auto& button : buttons) {
        for (const auto& platform : movingPlatforms) {
//            spdlog::info("Checking buttonId {} with platformId {} ({})", button->id, platform->id, platform->id % 100 == button->id % 100);
            if (platform->id % 100 == button->id % 100) {
                button->addPlatform(platform.get());
                spdlog::info("Button {} connected to Platform {}", button->id, platform->id);
            }
        }
        if (button->connectedPlatforms.empty()) {
            spdlog::warn("Button {} does not have a matching platform.", button->id);
        }
    }

    if (doorButton != nullptr) {
        winArea->OnDeactivate(); // TODO: verify logic
        doorButton->addWinArea(winArea);
    }

    for (const auto& cable: cables) { // TODO: verify if works in all cases, change if necessary
        for (const auto& button: buttons) {
            if (cable->id % 100 == button->id % 100) {
                button->connectCable(cable);
                spdlog::info("connected cable {} to button", cable->id, button->id);
            }
        }
        if (doorButton != nullptr && cable->id % 100 == doorButton->id % 100) { // the second operand is not evaluated if the first operand is false
            doorButton->connectCable(cable);
            spdlog::info("connected cable {} to door button", cable->id, doorButton->id);
        }
    }
//    if(buttons[1] != nullptr && buttons[0] != nullptr) // only for testing and example of connected buttons
//    {
//        buttons[0]->addConnectedButton(buttons[1]);
//        buttons[1]->addConnectedButton(buttons[0]);
//    }
    /*
    if(buttons[1] != nullptr && buttons[0] != nullptr) // only for testing and example of conditional buttons, meaning that only when both buttons are pressed they activate platform
    {
        buttons[0]->addConditionalButton(buttons[1]);
        buttons[1]->addConditionalButton(buttons[0]);
    }*/

    for (const auto& platform : movingPlatforms) {
        auto it = targetPositions.find(platform->id);
        if (it != targetPositions.end()) {
            platform->positionTarget = it->second - platform->modelMaxVertex + _transform._position;
            platform->positionTarget.z += platform->size.z;
          
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
};

void Level::Draw(Shader &shader) {
    //shader.use();
    if(_model == nullptr) {
        spdlog::error("null model in {}", tag);
        return;
    }

    _transform.updateWorldTransform(glm::mat4(1.0f), shader);
    // shader.setMat4("model", _transform.getModel());

    staticModel.Draw(shader);
}

void Level::LoadDataFromJson(const Level& temp) {
    this->playerGrabberStartingPos = temp.playerGrabberStartingPos;
    this->playerJumperStartingPos = temp.playerJumperStartingPos;
    this->cameraOffset = temp.cameraOffset;

    this->winArea->playerGrabberLightBulb = std::make_shared<LightBulb>();
    this->winArea->playerGrabberLightBulb->loadModel("../../res/models/Assets/lightBulb/untitled.obj");
    this->playerGrabberLightPos = temp.playerGrabberLightPos;
    this->winArea->playerGrabberLightBulb->_transform._position = temp.playerGrabberLightPos;
    this->winArea->playerJumperLightBulb = std::make_shared<LightBulb>();
    this->winArea->playerJumperLightBulb->loadModel("../../res/models/Assets/lightBulb/untitled.obj");
    this->playerJumperLightPos = temp.playerJumperLightPos;
    this->winArea->playerJumperLightBulb->_transform._position = temp.playerJumperLightPos;
    if(temp.hasConditionalLight)
    {
        this->winArea->activationLightBulb = std::make_shared<LightBulb>();
        this->winArea->activationLightBulb->loadModel("../../res/models/Assets/lightBulb/untitled.obj");
        this->winLightPos = temp.winLightPos;
        this->winArea->activationLightBulb->_transform._position = temp.winLightPos;
    }
    this->winArea->SetLights();
}

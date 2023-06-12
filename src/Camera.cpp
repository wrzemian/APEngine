#include "../include/Engine.h"
#include "../include/Camera.h"

float YAW = -90.0f;
float PITCH = 0.0f;
float SPEED = 2.5f;
float SENSITIVITY = 0.1f;
float ZOOM = 45.0f;
bool isBlocked = false;
bool isFound = false;

int SCR_WIDTH = Engine::SCR_WIDTH;
int SCR_HEIGHT = Engine::SCR_HEIGHT;


rapidjson::Document Camera::ParseToJSON() {
    rapidjson::Document d;
    d.SetObject();
    d.AddMember("type", "camera", d.GetAllocator());
    d.AddMember("posX", Position.x, d.GetAllocator());
    d.AddMember("posY", Position.y, d.GetAllocator());
    d.AddMember("posZ", Position.z, d.GetAllocator());
    d.AddMember("rotX", Yaw, d.GetAllocator());
    d.AddMember("rotY", Pitch, d.GetAllocator());
    d.AddMember("zoom", Zoom, d.GetAllocator());


    return d;
}

void Camera::ImGui(){

    ImGui::Begin("Camera");
    ImGui::SetWindowSize(ImVec2(300, 400));

    ImGui::SliderFloat("position X", &Position.x, -100.0f, 100.0f);
    ImGui::SliderFloat("position Y", &Position.y, -100.0f, 100.0f);
    ImGui::SliderFloat("position Z", &Position.z, -100.0f, 100.f);
    ImGui::SliderFloat("zoom", &Zoom, 0.0f, 100.f);
    ImGui::SliderFloat("rotate X", &degX, -180.0f, 180.0f);
    ImGui::SliderFloat("rotate Y", &degY, -180.0f, 180.0f);
    rotate(degX, degY);
    ImGui::Checkbox("Lock target point", &isBlocked);
    if (ImGui::Button("SAVE CAM")) {
        Engine::parser.SaveJSON(this->ParseToJSON(), "camera");
    }
    if(ImGui::Button("Calculate Lock target Point")) {
        Look = Look - Position;
    }
    ImGui::Checkbox("Follow object / reset kamery", &isFound);

    ImGui::End();
}

Camera::Camera(glm::vec3 position, float yaw, float pitch, float zoom)
: Front(glm::vec3(0.0f, 0.0f, -1.0f)), Yaw(yaw), Pitch(pitch), Zoom(zoom) {
    //spdlog::warn("camera .json constructor");
    Position = position;
    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    degX = yaw;
    degY = pitch;

    updateCameraVectors();
}

Camera::Camera(std::string fileName) {
    spdlog::info("camera JSON constructor");
    rapidjson::Document d = Engine::parser.openJSON(fileName);
    std::string type = d["type"].GetString();
    IGui::setWindowName("camera");

    if(type == "camera") {
        Position = glm::vec3(d["posX"].GetFloat(), d["posY"].GetFloat(), d["posZ"].GetFloat());
        Zoom = d["zoom"].GetFloat();
        Front = glm::vec3(0.0f, 0.0f, -1.0f);
        Yaw = d["rotX"].GetFloat();
        Pitch = d["rotY"].GetFloat();
        WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        degX = Yaw;
        degY = Pitch;
        positionOrigin = Position;
    }
    else
        spdlog::error("no JSON file found");
}

void Camera::Update(float deltaTime) {
    if (moveToTarget) {
        t += deltaTime * speed;
        if (t >= 1.0f) {
            t = 1.0f;
            moveToTarget = false;
            Position = (1.0f - t) * positionOrigin + t * positionTarget;
            positionOrigin = Position;
        }
        Position = (1.0f - t) * positionOrigin + t * positionTarget;
    }
}

void Camera::MoveToTarget(glm::vec3 targetPosition) {
    positionTarget = targetPosition;
    moveToTarget = true;
}

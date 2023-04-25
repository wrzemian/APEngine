#include "../../include/lights/DirectionalLight.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "../include/Engine.h"

DirectionalLight::DirectionalLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                                   const glm::vec3 &direction) : AmbientLight(ambient, diffuse, specular),
                                                                 direction(direction) {
    Engine::addDirLight(this);
}

DirectionalLight::DirectionalLight() {
    Engine::addDirLight(this);
}

DirectionalLight::~DirectionalLight() {
    Engine::removeDirLight(this);
}

const glm::vec3 &DirectionalLight::getDirection() const {
    return direction;
}

void DirectionalLight::setDirection(const glm::vec3 &direction) {
    DirectionalLight::direction = direction;
}

void DirectionalLight::SendToShader(Shader shader, std::string type) {
    AmbientLight::SendToShader(shader, type);
    shader.setVec3(type + ".direction", direction);
}





void DirectionalLight::setAmbient(const glm::vec3 &ambient) {
    AmbientLight::setAmbient(ambient);
}

void DirectionalLight::setDiffuse(const glm::vec3 &diffuse) {
    AmbientLight::setDiffuse(diffuse);
}

void DirectionalLight::setSpecular(const glm::vec3 &specular) {
    AmbientLight::setSpecular(specular);
}

void DirectionalLight::ImGui() {
    ImGui::Begin("Dir Lignt");
    ImGui::SetWindowSize(ImVec2(250, 150));

    ImGui::SliderFloat3("ambient", &ambient.x, -1.0f, 1.0f);
    ImGui::SliderFloat3("diffuse", &diffuse.x, -1.0f, 1.0f);
    ImGui::SliderFloat3("specular", &specular.x, -1.0f, 1.0f);
    ImGui::SliderFloat3("direction", &direction.x, -1.0f, 1.0f);

    if (ImGui::Button("SAVE")) {
        Engine::parser.SaveJSON(this->ParseToJSON(), "dirLight");
    }

    ImGui::End();
}

rapidjson::Document DirectionalLight::ParseToJSON() {
    rapidjson::Document d;
    d.SetObject();
    d.AddMember("type", "dirLight", d.GetAllocator());
    d.AddMember("ambientX", ambient.x, d.GetAllocator());
    d.AddMember("ambientY", ambient.y, d.GetAllocator());
    d.AddMember("ambientZ", ambient.z, d.GetAllocator());
    d.AddMember("diffuseX", diffuse.x, d.GetAllocator());
    d.AddMember("diffuseY", diffuse.y, d.GetAllocator());
    d.AddMember("diffuseZ", diffuse.z, d.GetAllocator());
    d.AddMember("specularX", specular.x, d.GetAllocator());
    d.AddMember("specularY", specular.y, d.GetAllocator());
    d.AddMember("specularZ", specular.z, d.GetAllocator());
    d.AddMember("directionX", direction.x, d.GetAllocator());
    d.AddMember("directionY", direction.y, d.GetAllocator());
    d.AddMember("directionZ", direction.z, d.GetAllocator());


    return d;
}

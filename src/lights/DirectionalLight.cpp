//
// Created by wrzos on 28.03.2023.
//

#include "../../include/lights/DirectionalLight.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"

DirectionalLight::DirectionalLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                                   const glm::vec3 &direction) : AmbientLight(ambient, diffuse, specular),
                                                                 direction(direction) {}

const glm::vec3 &DirectionalLight::getDirection() const {
    return direction;
}

void DirectionalLight::setDirection(const glm::vec3 &direction) {
    DirectionalLight::direction = direction;
}

void DirectionalLight::sendToShader(Shader shader, std::string type) {
    AmbientLight::sendToShader(shader, type);
    shader.setVec3(type + ".direction", direction);
}

DirectionalLight::~DirectionalLight() {

}

DirectionalLight::DirectionalLight() {

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
    ImGui::SetWindowSize(ImVec2(250, 130));

    ImGui::SliderFloat3("ambient", &ambient.x, -1.0f, 1.0f);
    ImGui::SliderFloat3("diffuse", &diffuse.x, -1.0f, 1.0f);
    ImGui::SliderFloat3("specular", &specular.x, -1.0f, 1.0f);
    ImGui::SliderFloat3("direction", &direction.x, -1.0f, 1.0f);

    ImGui::End();
}


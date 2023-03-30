//
// Created by wrzos on 28.03.2023.
//

#include "../../include/lights/SpotLight.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"

SpotLight::SpotLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                     const glm::vec3 &position, float constant, float linear, float quadratic,
                     const glm::vec3 &direction, float cutOff, float outerCutOff) : PointLight(ambient, diffuse,
                                                                                               specular, position,
                                                                                               constant, linear,
                                                                                               quadratic),
                                                                                    direction(direction),
                                                                                    cutOff(cutOff),
                                                                                    outerCutOff(outerCutOff) {}

SpotLight::~SpotLight() {

}

const glm::vec3 &SpotLight::getDirection() const {
    return direction;
}



void SpotLight::setDirection(const glm::vec3 &direction) {
    SpotLight::direction = direction;
}



void SpotLight::sendToShader(Shader shader, std::string type) {
    PointLight::sendToShader(shader, type);
    shader.setVec3(type + ".direction", direction);
    shader.setFloat(type + ".cutOff", cutOff);
    shader.setFloat(type + ".outerCutOff", outerCutOff);
}

SpotLight::SpotLight() {

}

float SpotLight::getCutOff() const {
    return cutOff;
}

float SpotLight::getOuterCutOff() const {
    return outerCutOff;
}

void SpotLight::setCutOff(float cutOff) {
    SpotLight::cutOff = cutOff;
}

void SpotLight::setOuterCutOff(float outerCutOff) {
    SpotLight::outerCutOff = outerCutOff;
}

void SpotLight::setAmbient(const glm::vec3 &ambient) {
    PointLight::setAmbient(ambient);
}

void SpotLight::setDiffuse(const glm::vec3 &diffuse) {
    PointLight::setDiffuse(diffuse);
}

void SpotLight::setSpecular(const glm::vec3 &specular) {
    PointLight::setSpecular(specular);
}

void SpotLight::setPosition(const glm::vec3 &position) {
    PointLight::setPosition(position);
}

void SpotLight::setConstant(float constant) {
    PointLight::setConstant(constant);
}

void SpotLight::setLinear(float linear) {
    PointLight::setLinear(linear);
}

void SpotLight::setQuadratic(float quadratic) {
    PointLight::setQuadratic(quadratic);
}

void SpotLight::ImGui() {
    ImGui::Begin("Spot Lignt");
    ImGui::SetWindowSize(ImVec2(250, 300));

    ImGui::SliderFloat3("ambient", &ambient.x, -1.0f, 1.0f);
    ImGui::SliderFloat3("diffuse", &diffuse.x, -1.0f, 1.0f);
    ImGui::SliderFloat3("specular", &specular.x, -1.0f, 1.0f);
    ImGui::SliderFloat3("position", &position.x, -5.0f, 5.0f);
    ImGui::SliderFloat("constant", &constant, -1.0f, 1.0f);
    ImGui::SliderFloat("linear", &linear, -1.0f, 1.0f);
    ImGui::SliderFloat("quadratic", &quadratic, -1.0f, 1.0f);
    ImGui::SliderFloat3("direction", &direction.x, -1.0f, 1.0f);
    ImGui::SliderFloat("cutOff", &cutOff, -1.0f, 1.0f);
    ImGui::SliderFloat("outerCutOff", &outerCutOff, -1.0f, 1.0f);
    ImGui::End();
}



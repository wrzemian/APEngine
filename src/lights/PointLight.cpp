//
// Created by wrzos on 28.03.2023.
//

#include "../../include/lights/PointLight.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"

PointLight::PointLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                       const glm::vec3 &position, float constant, float linear, float quadratic) : AmbientLight(ambient,
                                                                                                                diffuse,
                                                                                                                specular),
                                                                                                   position(position),
                                                                                                   constant(constant),
                                                                                                   linear(linear),
                                                                                                   quadratic(
                                                                                                           quadratic) {}

PointLight::~PointLight() {

}

const glm::vec3 &PointLight::getPosition() const {
    return position;
}



void PointLight::setPosition(const glm::vec3 &position) {
    PointLight::position = position;
}



void PointLight::sendToShader(Shader shader, std::string type) {
    AmbientLight::sendToShader(shader, type);
    shader.setVec3(type + ".position", position);
    shader.setFloat(type + ".constant", constant);
    shader.setFloat(type + ".linear", linear);
    shader.setFloat(type + ".quadratic", quadratic);
}

PointLight::PointLight() {}

float PointLight::getConstant() const {
    return constant;
}

float PointLight::getLinear() const {
    return linear;
}

float PointLight::getQuadratic() const {
    return quadratic;
}

void PointLight::setConstant(float constant) {
    PointLight::constant = constant;
}

void PointLight::setLinear(float linear) {
    PointLight::linear = linear;
}

void PointLight::setQuadratic(float quadratic) {
    PointLight::quadratic = quadratic;
}

void PointLight::setAmbient(const glm::vec3 &ambient) {
    AmbientLight::setAmbient(ambient);
}

void PointLight::setDiffuse(const glm::vec3 &diffuse) {
    AmbientLight::setDiffuse(diffuse);
}

void PointLight::setSpecular(const glm::vec3 &specular) {
    AmbientLight::setSpecular(specular);
}

void PointLight::ImGui() {
    ImGui::Begin("Point Lignt");
    ImGui::SetWindowSize(ImVec2(250, 300));

    ImGui::SliderFloat3("ambient", &ambient.x, -1.0f, 1.0f);
    ImGui::SliderFloat3("diffuse", &diffuse.x, -1.0f, 1.0f);
    ImGui::SliderFloat3("specular", &specular.x, -1.0f, 1.0f);
    ImGui::SliderFloat3("position", &position.x, -5.0f, 5.0f);
    ImGui::SliderFloat("constant", &constant, -1.0f, 1.0f);
    ImGui::SliderFloat("linear", &linear, -1.0f, 1.0f);
    ImGui::SliderFloat("quadratic", &quadratic, -1.0f, 1.0f);
    ImGui::End();
}


//
// Created by wrzos on 28.03.2023.
//

#include "../../include/lights/PointLight.h"

PointLight::PointLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                       const glm::vec3 &position, const glm::vec3 &constant, const glm::vec3 &linear,
                       const glm::vec3 &quadratic) : AmbientLight(ambient, diffuse, specular), position(position),
                                                     constant(constant), linear(linear), quadratic(quadratic) {}

PointLight::~PointLight() {

}

const glm::vec3 &PointLight::getPosition() const {
    return position;
}

const glm::vec3 &PointLight::getConstant() const {
    return constant;
}

const glm::vec3 &PointLight::getLinear() const {
    return linear;
}

const glm::vec3 &PointLight::getQuadratic() const {
    return quadratic;
}

void PointLight::setPosition(const glm::vec3 &position) {
    PointLight::position = position;
}

void PointLight::setConstant(const glm::vec3 &constant) {
    PointLight::constant = constant;
}

void PointLight::setLinear(const glm::vec3 &linear) {
    PointLight::linear = linear;
}

void PointLight::setQuadratic(const glm::vec3 &quadratic) {
    PointLight::quadratic = quadratic;
}

void PointLight::sendToShader(Shader shader, std::string type) {
    AmbientLight::sendToShader(shader, type);
    shader.setVec3(type + ".position", position);
    shader.setVec3(type + ".constant", constant);
    shader.setVec3(type + ".linear", linear);
    shader.setVec3(type + ".quadratic", quadratic);
}

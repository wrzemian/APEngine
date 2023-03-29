//
// Created by wrzos on 28.03.2023.
//

#include "../../include/lights/PointLight.h"

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



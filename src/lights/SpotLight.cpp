//
// Created by wrzos on 28.03.2023.
//

#include "../../include/lights/SpotLight.h"

SpotLight::SpotLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                     const glm::vec3 &position, const glm::vec3 &constant, const glm::vec3 &linear,
                     const glm::vec3 &quadratic, const glm::vec3 &direction, const glm::vec3 &cutOff,
                     const glm::vec3 &outerCutOff) : PointLight(ambient, diffuse, specular, position, constant, linear,
                                                                quadratic), direction(
        direction), cutOff(cutOff), outerCutOff(outerCutOff) {}

SpotLight::~SpotLight() {

}

const glm::vec3 &SpotLight::getDirection() const {
    return direction;
}

const glm::vec3 &SpotLight::getCutOff() const {
    return cutOff;
}

const glm::vec3 &SpotLight::getOuterCutOff() const {
    return outerCutOff;
}

void SpotLight::setDirection(const glm::vec3 &direction) {
    SpotLight::direction = direction;
}

void SpotLight::setCutOff(const glm::vec3 &cutOff) {
    SpotLight::cutOff = cutOff;
}

void SpotLight::setOuterCutOff(const glm::vec3 &outerCutOff) {
    SpotLight::outerCutOff = outerCutOff;
}

void SpotLight::sendToShader(Shader shader, std::string type) {
    PointLight::sendToShader(shader, type);
    shader.setVec3(type + ".direction", direction);
    shader.setVec3(type + ".cutOff", cutOff);
    shader.setVec3(type + ".outerCutOff", outerCutOff);
}

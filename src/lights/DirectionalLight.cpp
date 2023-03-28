//
// Created by wrzos on 28.03.2023.
//

#include "../../include/lights/DirectionalLight.h"

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


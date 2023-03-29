//
// Created by wrzos on 28.03.2023.
//

#include "../../include/lights/AmbientLight.h"

AmbientLight::AmbientLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular) : ambient(ambient),
                                                                                              diffuse(diffuse),
                                                                                              specular(specular) {}

AmbientLight::~AmbientLight() {

}

const glm::vec3 &AmbientLight::getAmbient() const {
    return ambient;
}

const glm::vec3 &AmbientLight::getDiffuse() const {
    return diffuse;
}

const glm::vec3 &AmbientLight::getSpecular() const {
    return specular;
}

void AmbientLight::setAmbient(const glm::vec3 &ambient) {
    AmbientLight::ambient = ambient;
}

void AmbientLight::setDiffuse(const glm::vec3 &diffuse) {
    AmbientLight::diffuse = diffuse;
}

void AmbientLight::setSpecular(const glm::vec3 &specular) {
    AmbientLight::specular = specular;
}

void AmbientLight::sendToShader(Shader shader, std::string type) {
    shader.setVec3(type + ".ambient", ambient);
    shader.setVec3(type + ".diffuse", diffuse);
    shader.setVec3(type + ".specular", specular);
}

AmbientLight::AmbientLight() {}


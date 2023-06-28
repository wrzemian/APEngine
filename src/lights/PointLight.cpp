#include "../../include/lights/PointLight.h"
#include "../include/Engine.h"

PointLight::PointLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                       const glm::vec3 &position, float constant, float linear, float quadratic) : AmbientLight(ambient,
                                                                                                                diffuse,
                                                                                                                specular),
                                                                                                   position(position),
                                                                                                   constant(constant),
                                                                                                   linear(linear),
                                                                                                   quadratic(
                                                                                                           quadratic) {
    Engine::addPointLight(this);
}

PointLight::PointLight() {
    Engine::addPointLight(this);
}

PointLight::~PointLight() {
    Engine::removePointLight(this);
}

const glm::vec3 &PointLight::getPosition() const {
    return position;
}



void PointLight::setPosition(const glm::vec3 &position) {
    PointLight::position = position;
}



void PointLight::SendToShader(Shader shader, std::string type) {
    AmbientLight::SendToShader(shader, type);
    shader.setVec3(type + ".position", position);
    shader.setFloat(type + ".constant", constant);
    shader.setFloat(type + ".linear", linear);
    shader.setFloat(type + ".quadratic", quadratic);
}



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



rapidjson::Document PointLight::ParseToJSON() {
    rapidjson::Document d;
    d.SetObject();
    d.AddMember("type", "pointLight", d.GetAllocator());
    d.AddMember("ambientX", ambient.x, d.GetAllocator());
    d.AddMember("ambientY", ambient.y, d.GetAllocator());
    d.AddMember("ambientZ", ambient.z, d.GetAllocator());
    d.AddMember("diffuseX", diffuse.x, d.GetAllocator());
    d.AddMember("diffuseY", diffuse.y, d.GetAllocator());
    d.AddMember("diffuseZ", diffuse.z, d.GetAllocator());
    d.AddMember("specularX", specular.x, d.GetAllocator());
    d.AddMember("specularY", specular.y, d.GetAllocator());
    d.AddMember("specularZ", specular.z, d.GetAllocator());
    d.AddMember("positionX", position.x, d.GetAllocator());
    d.AddMember("positionY", position.y, d.GetAllocator());
    d.AddMember("positionZ", position.z, d.GetAllocator());
    d.AddMember("constant", constant, d.GetAllocator());
    d.AddMember("linear", linear, d.GetAllocator());
    d.AddMember("quadratic", quadratic, d.GetAllocator());


    return d;
}
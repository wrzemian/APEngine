#include "../../include/lights/SpotLight.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"
#include "../../include/Engine.h"

SpotLight::SpotLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                     const glm::vec3 &position, float constant, float linear, float quadratic,
                     const glm::vec3 &direction, float cutOff, float outerCutOff) : PointLight(ambient, diffuse,
                                                                                               specular, position,
                                                                                               constant, linear,
                                                                                               quadratic),
                                                                                    direction(direction),
                                                                                    cutOff(cutOff),
                                                                                    outerCutOff(outerCutOff) {
    Engine::addSpotLight(getSharedSpotLight());
}

SpotLight::SpotLight() {
    Engine::addSpotLight(getSharedSpotLight());
}

SpotLight::~SpotLight() {
    Engine::removeSpotLight(getSharedSpotLight());
}

const glm::vec3 &SpotLight::getDirection() const {
    return direction;
}



void SpotLight::setDirection(const glm::vec3 &direction) {
    SpotLight::direction = direction;
}



void SpotLight::SendToShader(Shader shader, std::string type) {
    PointLight::SendToShader(shader, type);
    shader.setVec3(type + ".direction", direction);
    shader.setFloat(type + ".cutOff", cutOff);
    shader.setFloat(type + ".outerCutOff", outerCutOff);
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

    if (ImGui::Button("SAVE")) {
        Engine::parser.SaveJSON(this->ParseToJSON(), "lights/spotLight");
    }
    ImGui::End();
}

rapidjson::Document SpotLight::ParseToJSON() {
    rapidjson::Document d;
    d.SetObject();
    d.AddMember("type", "spotLight", d.GetAllocator());
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
    d.AddMember("directionX", direction.x, d.GetAllocator());
    d.AddMember("directionY", direction.y, d.GetAllocator());
    d.AddMember("directionZ", direction.z, d.GetAllocator());
    d.AddMember("cutOff", cutOff, d.GetAllocator());
    d.AddMember("outerCutOff", outerCutOff, d.GetAllocator());


    return d;
}

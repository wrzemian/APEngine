//
// Created by wrzos on 28.03.2023.
//

#ifndef APENGINE_POINTLIGHT_H
#define APENGINE_POINTLIGHT_H

#include "AmbientLight.h"

class PointLight : public AmbientLight{
protected:
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;

public:
    PointLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, const glm::vec3 &position,
               float constant, float linear, float quadratic);

    PointLight();

    ~PointLight() override;

    const glm::vec3 &getPosition() const;

    float getConstant() const;

    float getLinear() const;

    float getQuadratic() const;

    void setAmbient(const glm::vec3 &ambient);

    void setDiffuse(const glm::vec3 &diffuse);

    void setSpecular(const glm::vec3 &specular);

    void setPosition(const glm::vec3 &position);

    void setConstant(float constant);

    void setLinear(float linear);

    void setQuadratic(float quadratic);

    void ImGui();

    void SendToShader(Shader shader, std::string type) override;

    rapidjson::Document ParseToJSON();
};


#endif //APENGINE_POINTLIGHT_H

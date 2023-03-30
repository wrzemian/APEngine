//
// Created by wrzos on 28.03.2023.
//

#ifndef APENGINE_SPOTLIGHT_H
#define APENGINE_SPOTLIGHT_H

#include "PointLight.h"
#include "glm/vec3.hpp"

class SpotLight : PointLight{
protected:
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;

public:
    SpotLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, const glm::vec3 &position,
              float constant, float linear, float quadratic, const glm::vec3 &direction, float cutOff,
              float outerCutOff);

    virtual ~SpotLight();

    const glm::vec3 &getDirection() const;

    float getCutOff() const;

    float getOuterCutOff() const;

    void setDirection(const glm::vec3 &direction);

    void setCutOff(float cutOff);

    void setOuterCutOff(float outerCutOff);

    void setAmbient(const glm::vec3 &ambient);

    void setDiffuse(const glm::vec3 &diffuse);

    void setSpecular(const glm::vec3 &specular);

    void setPosition(const glm::vec3 &position);

    void setConstant(float constant);

    void setLinear(float linear);

    void setQuadratic(float quadratic);

    void sendToShader(Shader shader, std::string type) override;

    void ImGui();

    SpotLight();
};


#endif //APENGINE_SPOTLIGHT_H

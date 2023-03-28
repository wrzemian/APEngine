//
// Created by wrzos on 28.03.2023.
//

#ifndef APENGINE_SPOTLIGHT_H
#define APENGINE_SPOTLIGHT_H

#include "PointLight.h"
#include "glm/vec3.hpp"

class SpotLight : PointLight{
private:
    glm::vec3 direction;
    glm::vec3 cutOff;
    glm::vec3 outerCutOff;

public:
    SpotLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
              const glm::vec3 &position, const glm::vec3 &constant, const glm::vec3 &linear,
              const glm::vec3 &quadratic, const glm::vec3 &direction, const glm::vec3 &cutOff,
              const glm::vec3 &outerCutOff);

    virtual ~SpotLight();

    const glm::vec3 &getDirection() const;

    const glm::vec3 &getCutOff() const;

    const glm::vec3 &getOuterCutOff() const;

    void setDirection(const glm::vec3 &direction);

    void setCutOff(const glm::vec3 &cutOff);

    void setOuterCutOff(const glm::vec3 &outerCutOff);

    void sendToShader(Shader shader, std::string type) override;
};


#endif //APENGINE_SPOTLIGHT_H

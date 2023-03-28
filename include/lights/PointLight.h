//
// Created by wrzos on 28.03.2023.
//

#ifndef APENGINE_POINTLIGHT_H
#define APENGINE_POINTLIGHT_H

#include "AmbientLight.h"

class PointLight : AmbientLight{
private:
    glm::vec3 position;
    glm::vec3 constant;
    glm::vec3 linear;
    glm::vec3 quadratic;

public:
    PointLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular, const glm::vec3 &position,
               const glm::vec3 &constant, const glm::vec3 &linear, const glm::vec3 &quadratic);

    ~PointLight() override;

    const glm::vec3 &getPosition() const;

    const glm::vec3 &getConstant() const;

    const glm::vec3 &getLinear() const;

    const glm::vec3 &getQuadratic() const;

    void setPosition(const glm::vec3 &position);

    void setConstant(const glm::vec3 &constant);

    void setLinear(const glm::vec3 &linear);

    void setQuadratic(const glm::vec3 &quadratic);

    void sendToShader(Shader shader, std::string type) override;
};


#endif //APENGINE_POINTLIGHT_H

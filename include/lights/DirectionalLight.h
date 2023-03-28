//
// Created by wrzos on 28.03.2023.
//

#ifndef APENGINE_DIRECTIONALLIGHT_H
#define APENGINE_DIRECTIONALLIGHT_H

#include "AmbientLight.h"

class DirectionalLight : AmbientLight{
private:
    glm::vec3 direction;

public:
    DirectionalLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                     const glm::vec3 &direction);

    ~DirectionalLight() override;

    const glm::vec3 &getDirection() const;

    void setDirection(const glm::vec3 &direction);

    void sendToShader(Shader shader, std::string type) override;

};


#endif //APENGINE_DIRECTIONALLIGHT_H

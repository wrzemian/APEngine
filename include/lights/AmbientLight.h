//
// Created by wrzos on 28.03.2023.
//

#ifndef APENGINE_AMBIENTLIGHT_H
#define APENGINE_AMBIENTLIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Shader.h"
#include "document.h"
#include "ILight.h"

class AmbientLight : public ILight {
protected:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

public:
    AmbientLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular);

    AmbientLight();

    virtual ~AmbientLight();

    const glm::vec3 &getAmbient() const;

    const glm::vec3 &getDiffuse() const;

    const glm::vec3 &getSpecular() const;

    void setAmbient(const glm::vec3 &ambient);

    void setDiffuse(const glm::vec3 &diffuse);

    void setSpecular(const glm::vec3 &specular);

    virtual void sendToShader(Shader shader, std::string type);

    rapidjson::Document parseToJSON();

};


#endif //APENGINE_AMBIENTLIGHT_H

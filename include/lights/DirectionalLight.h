//
// Created by wrzos on 28.03.2023.
//

#ifndef APENGINE_DIRECTIONALLIGHT_H
#define APENGINE_DIRECTIONALLIGHT_H

#include "AmbientLight.h"
#include "../IGui.h"


class DirectionalLight : public AmbientLight, public std::enable_shared_from_this<DirectionalLight>{
protected:
    glm::vec3 direction;

public:
    DirectionalLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                     const glm::vec3 &direction);

    ~DirectionalLight() override;

    void setAmbient(const glm::vec3 &ambient);

    void setDiffuse(const glm::vec3 &diffuse);

    void setSpecular(const glm::vec3 &specular);

    const glm::vec3 &getDirection() const;

    void setDirection(const glm::vec3 &direction);

    void SendToShader(Shader shader, std::string type) override;

    rapidjson::Document ParseToJSON();


    DirectionalLight();

    void ImGui();
};


#endif //APENGINE_DIRECTIONALLIGHT_H

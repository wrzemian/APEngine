//
// Created by wrzos on 29.05.2023.
//

#ifndef APENGINE_SHADOWS_H
#define APENGINE_SHADOWS_H


#include "document.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "../Shader.h"
#include "../Camera.h"

class Shadows {
public:
    glm::vec3 position;
    glm::vec2 orthoHor;
    glm::vec2 orthoVer;
    glm::vec3 idk;
    glm::vec3 lookAt;
    float near_plane;
    float far_plane;
    unsigned int SHADOW_WIDTH;
    unsigned int SHADOW_HEIGHT;
    unsigned int depthMapFBO;
    unsigned int depthMap;
    float time;
    Shader shader;
    Shader simpleDepthShader;
    glm::vec3 cameraPosition;

    Shadows();
    Shadows(std::string fileName);
    void initShaders(Camera& camera);
    void renderShadows(Camera& camera);

    rapidjson::Document ParseToJSON();
};


#endif //APENGINE_SHADOWS_H

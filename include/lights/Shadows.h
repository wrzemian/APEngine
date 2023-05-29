//
// Created by wrzos on 29.05.2023.
//

#ifndef APENGINE_SHADOWS_H
#define APENGINE_SHADOWS_H


#include "../IGui.h"
#include "document.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "../Shader.h"
#include "../Camera.h"

class Shadows : public IGui {
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
    Shader shader;
    Shader simpleDepthShader;

    Shadows();
    Shadows(std::string fileName);
    void initShaders();
    void renderShadows(Camera& camera);

    void ImGui() override;
    rapidjson::Document ParseToJSON();
};


#endif //APENGINE_SHADOWS_H
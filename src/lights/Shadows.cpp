//
// Created by wrzos on 29.05.2023.
//

#include "../../include/lights/Shadows.h"
#include "../../include/Engine.h"

Shadows::Shadows() {
    IGui::setWindowName("shadows");
    position = glm::vec3(-5.0f, 11.370, 15.130f);
    orthoHor = glm::vec2(-10.0f, 10.0f);
    orthoVer = glm::vec2(-10.0f, 10.0f);
    idk = glm::vec3(0.0f, 0.0f, 0.0f);
    lookAt = glm::vec3(0.0f, 1.0f, 0.0f);
    near_plane = 0.1f;
    far_plane = 30.5f;
    SHADOW_WIDTH = 2048;
    SHADOW_HEIGHT = 2048;
}

Shadows::Shadows(std::string fileName) {
    spdlog::info("shadows JSON constructor");
    rapidjson::Document d = Engine::parser.openJSON(fileName);
    std::string type = d["type"].GetString();
    IGui::setWindowName("shadows");

    if(type == "shadows") {
        position = glm::vec3(d["posX"].GetFloat(), d["posY"].GetFloat(), d["posZ"].GetFloat());
        orthoHor = glm::vec2(d["orthoHorX"].GetFloat(), d["orthoHorY"].GetFloat());
        orthoVer = glm::vec2(d["orthoVerX"].GetFloat(), d["orthoVerY"].GetFloat());
        idk = glm::vec3(d["idkX"].GetFloat(), d["idkY"].GetFloat(), d["idkZ"].GetFloat());
        lookAt = glm::vec3(d["lookAtX"].GetFloat(), d["lookAtY"].GetFloat(), d["lookAtZ"].GetFloat());
        near_plane = d["nearPlane"].GetFloat();
        far_plane = d["farPlane"].GetFloat();
        SHADOW_WIDTH = d["SHADOW_WIDTH"].GetInt();
        SHADOW_HEIGHT = d["SHADOW_HEIGHT"].GetInt();
    }
    else
        spdlog::error("no JSON file found");
}

void Shadows::initShaders() {
    Shader temp("../../res/shaders/shadows/shadowShader.vert", "../../res/shaders/shadows/shadowShader.frag");
    shader = temp;
    Shader tempSimpleDepthShader("../../res/shaders/shadows/depthShader.vert", "../../res/shaders/shadows/depthShader.frag");
    simpleDepthShader = tempSimpleDepthShader;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    shader.use();
    shader.setInt("diffuseTexture", 0);
    shader.setInt("shadowMap", 31);
}

void Shadows::ImGui()  {
    ImGui::Begin(getWindowName().c_str());
    ImGui::SetWindowSize(ImVec2(250, 400));

    ImGui::SliderFloat("lightX", &position.x, -50.0f, 50.0f);
    ImGui::SliderFloat("lightY", &position.y, -50.0f, 50.0f);
    ImGui::SliderFloat("lightZ", &position.z, -50.0f, 50.0f);
    ImGui::SliderFloat("idkX", &idk.x, -50.0f, 50.0f);
    ImGui::SliderFloat("idkY", &idk.y, -50.0f, 50.0f);
    ImGui::SliderFloat("idkZ", &idk.z, -50.0f, 50.0f);
    ImGui::SliderFloat("lookAtX", &lookAt.x, -50.0f, 50.0f);
    ImGui::SliderFloat("lookAtY", &lookAt.y, -50.0f, 50.0f);
    ImGui::SliderFloat("lookAtZ", &lookAt.z, -50.0f, 50.0f);
    ImGui::SliderFloat("nearPlane", &near_plane, -50.0f, 50.0f);
    ImGui::SliderFloat("farPlane", &far_plane, -50.0f, 50.0f);
    ImGui::SliderFloat("orthoHorX", &orthoHor.x, -50.0f, 50.0f);
    ImGui::SliderFloat("orthoHorY", &orthoHor.y, -50.0f, 50.0f);
    ImGui::SliderFloat("orthoVerX", &orthoVer.x, -50.0f, 50.0f);
    ImGui::SliderFloat("orthoVerY", &orthoVer.y, -50.0f, 50.0f);

    if (ImGui::Button("SAVE SHADOWS")) {

        Engine::parser.SaveJSON(this->ParseToJSON(), "lights/shadows");
    }
    ImGui::End();

}

rapidjson::Document Shadows::ParseToJSON() {
    rapidjson::Document d;
    d.SetObject();
    d.AddMember("type", "shadows", d.GetAllocator());
    d.AddMember("posX", position.x, d.GetAllocator());
    d.AddMember("posY", position.y, d.GetAllocator());
    d.AddMember("posZ", position.z, d.GetAllocator());
    d.AddMember("idkX", idk.x, d.GetAllocator());
    d.AddMember("idkY", idk.y, d.GetAllocator());
    d.AddMember("idkZ", idk.z, d.GetAllocator());
    d.AddMember("lookAtX", lookAt.x, d.GetAllocator());
    d.AddMember("lookAtY", lookAt.y, d.GetAllocator());
    d.AddMember("lookAtZ", lookAt.z, d.GetAllocator());
    d.AddMember("nearPlane", near_plane, d.GetAllocator());
    d.AddMember("farPlane", far_plane, d.GetAllocator());
    d.AddMember("orthoHorX", orthoHor.x, d.GetAllocator());
    d.AddMember("orthoHorY", orthoHor.y, d.GetAllocator());
    d.AddMember("orthoVerX", orthoVer.x, d.GetAllocator());
    d.AddMember("orthoVerY", orthoVer.y, d.GetAllocator());
    d.AddMember("SHADOW_WIDTH", SHADOW_WIDTH, d.GetAllocator());
    d.AddMember("SHADOW_HEIGHT", SHADOW_HEIGHT, d.GetAllocator());

    return d;
}

void Shadows::renderShadows(Camera& camera) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 1. render depth of scene to texture (from light's perspective)
    // --------------------------------------------------------------
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    lightProjection = glm::ortho(orthoHor.x, orthoHor.y, orthoVer.x, orthoVer.y, near_plane, far_plane);
    lightView = glm::lookAt(position, idk, lookAt);
    lightSpaceMatrix = lightProjection * lightView;
    // render scene from light's point of view
    simpleDepthShader.use();
    simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    Engine::drawObjects(simpleDepthShader, camera);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // reset viewport
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    // set light uniforms
    shader.setVec3("viewPos", camera.Position);
    shader.setVec3("lightPos", position);
    shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    glActiveTexture(GL_TEXTURE31);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    Engine::drawObjects(shader, camera);
}



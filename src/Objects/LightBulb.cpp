//
// Created by Karol on 18.06.2023.
//

#include "../../include/Objects/LightBulb.h"

LightBulb::LightBulb() {
    _transform._scale.x = 0.5f;
    _transform._scale.y = 0.5f;
    _transform._scale.z = 0.5f;
    _transform._rotation.z = 1.57079632679f;
    Shader temp("../../res/shaders/colorShader.vert", "../../res/shaders/colorShader.frag");
    changingColorShader = temp;
    _shader = &temp;
    ChangeColor(glm::vec3(0,0,1));

    IGui::setWindowName("Light Bulb ");
}

void LightBulb::ChangeColor(glm::vec3 color) {
    std::cout << "light bulb changed color to : " << color.x << ", " <<  color.y << ", " << color.z << std::endl;
    materialColor = color;
}

void LightBulb::Draw(Shader &shader) {
    if(_model == nullptr) {
        spdlog::error("null model in {}", tag);
        return;
    }
    /*changingColorShader.use();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 16 / 9, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    changingColorShader.setMat4("projection", projection);
    changingColorShader.setMat4("view", view);
    changingColorShader.setVec3("inputColor",materialColor);*/
    //shader.setVec3("inColor",materialColor);
    //_transform.updateWorldTransform(glm::mat4(1.0f), shader);
    //_model->Draw(shader);

    //shader.setVec3("inColor", glm::vec3(0));

}


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
    ChangeColor(glm::vec3(1,1,1));
    ShowImgui();
}

void LightBulb::ChangeColor(glm::vec3 color) {
    std::cout << "light bulb changed color to : " << color.x << ", " <<  color.y << ", " << color.z << std::endl;
    changingColorShader.setVec3("inputColor",materialColor);
    materialColor = color;
}

void LightBulb::Draw(Shader &shader) {
    if(_model == nullptr) {
        spdlog::error("null model in {}", tag);
        return;
    }
    _transform.updateWorldTransform(glm::mat4(1.0f), changingColorShader);
    _model->Draw(changingColorShader);

}

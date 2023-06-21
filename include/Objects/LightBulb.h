//
// Created by Karol on 18.06.2023.
//

#ifndef APENGINE_LIGHTBULB_H
#define APENGINE_LIGHTBULB_H
#include "Object3D.h"

class LightBulb : public Object3D {
private:
    Shader changingColorShader;
public:
    glm::vec3 materialColor = glm::vec3(0);
    LightBulb();
    void ChangeColor(glm::vec3 color);
    void Draw(Shader &shader) override;
};


#endif //APENGINE_LIGHTBULB_H

//
// Created by tomek on 17.04.2023.
//

#ifndef APENGINE_ANIMATION_H
#define APENGINE_ANIMATION_H

#include <iostream>
#include "glad/glad.h"
#include "Shader.h"

class Animation{
public:
    Animation();
    void initAnimation();

    void renderAnimation(float time);
private:
   GLuint animationShaderProgram;
    GLuint animationVAO, animationVBO, animationTexture;
    float animationTime;

};


#endif //APENGINE_ANIMATION_H

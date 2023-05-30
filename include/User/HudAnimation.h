#ifndef APENGINE_HUDANIMATION_H
#define APENGINE_HUDANIMATION_H

#include <iostream>
#include "glad/glad.h"
#include "../Shader.h"



class HudAnimation{
public:
    HudAnimation(Shader _animationShader);

    HudAnimation();

    void initAnimation();

    void renderAnimation(float time, float x, float y, float z);
private:
    Shader animationShader;
    GLuint animationShaderProgram;
    GLuint animationVAO, animationVBO, animationTexture;
    float animationTime;


};


#endif //APENGINE_HUDANIMATION_H
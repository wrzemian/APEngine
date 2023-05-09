#ifndef APENGINE_ANIMATION_H
#define APENGINE_ANIMATION_H

#include <iostream>
#include "glad/glad.h"
#include "Shader.h"



class Animation{
public:
    Animation(Shader _animationShader);

    Animation();

    void initAnimation();

    void renderAnimation(float time, float x, float y, float z);
private:
    Shader animationShader;
    GLuint animationShaderProgram;
    GLuint animationVAO, animationVBO, animationTexture;
    float animationTime;


};


#endif //APENGINE_ANIMATION_H
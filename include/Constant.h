#ifndef APENGINE_CONSTANT_H
#define APENGINE_CONSTANT_H

#include <iostream>
#include "glad/glad.h"
#include "Shader.h"



class Constant {
private:
    Shader constShader;
    GLuint constVAO, constVBO, constTexture;
    float constTime;

public:
    Constant(const Shader _constShader);


    void initConstant();
    void renderConstant(double x, double y, double z);

    Constant();
};

#endif //APENGINE_CONSTANT_H
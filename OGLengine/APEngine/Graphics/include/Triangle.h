#pragma once
//#include <glfw3.h>
#include <string>
#include "Shader.cpp"

class Triangle
{
private:
    float vertices[18] = {
        // pozycje          // kolory
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // dolny prawy
       -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // dolny lewy
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // górny 
    };

    const char* vertexShaderPath =   "../OGLengine/APEngine/Graphics/res/shaders/shader.vs";
    const char* fragmentShaderPath = "../OGLengine/APEngine/Graphics/res/shaders/shader.fs";

    Shader* shader;

    unsigned int _VBO, _VAO;

    void bindBuffers();

public:
    void init();
    void draw();
};


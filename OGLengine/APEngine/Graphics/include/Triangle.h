#pragma once
#include <glfw3.h>
#include <string>
#include "Shader.cpp"

class Triangle
{
private:
    GLfloat vertices[9] = {
                        -0.5f, -0.5f, 0.0f,
                         0.5f, -0.5f, 0.0f,
                         0.0f,  0.5f, 0.0f
    };

    const char* vertexShaderPath = "res/shaders/shader.vs";
    const char* fragmentShaderPath = "res/shaders/shader.fs";

    Shader* shader;

    unsigned int _VBO, _VAO;

    void bindBuffers();

public:
    void init();
    void draw();
};


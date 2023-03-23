#pragma once
//#include <glfw3.h>
#include <string>
#include "Shader.cpp"
#include "Camera.h"

class Triangle
{
private:
    float backup[18] = {
        // pozycje          // kolory
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // dolny prawy
       -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // dolny lewy
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // górny 
    };

    float vertices[216] = {
        //position            //normals            
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   

        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  

         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f  
    };

    const char* vertexShaderPath =   "../OGLengine/APEngine/Graphics/res/shaders/shader.vs";
    const char* fragmentShaderPath = "../OGLengine/APEngine/Graphics/res/shaders/shader.fs";

    Shader* shader;

    unsigned int _VBO, _VAO;

    void bindBuffers();

public:
    void init();
    void draw(Camera camera);
};


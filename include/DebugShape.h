//
// Created by micha on 03.04.2023.
//
#pragma once
#include "Model.h"
#include "Shader.h"
//#include "imgui_impl/imgui_impl_opengl3_loader.h"

class DebugShape
{
protected:
    static GLuint VAO;
    static GLuint VBO;
    static GLuint EBO;
    static Model spotModel;

public:
    static Shader shader;

    static glm::mat4 world;
    static void Initialize();
    static void DrawSpot(glm::vec3 position, glm::vec4 color, glm::mat4 proview);
    static void DrawCube();
    static void DrawArrow(glm::vec3 Start, glm::vec3 End, glm::vec4 Color, glm::mat4 proview);

};

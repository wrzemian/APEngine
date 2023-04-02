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
    static Shader shader;
    static Model spotModel;

public:
    static glm::mat4 world;
    static void Initialize();
    static void DrawSpot(glm::vec3 position, glm::vec4 color, glm::mat4 proview);
    static void DrawCube(glm::vec3 position, glm::vec3 size, glm::vec4 color, glm::mat4 proview);
    static void DrawArrow(glm::vec3 Start, glm::vec3 End, glm::vec4 Color, glm::mat4 proview);
};

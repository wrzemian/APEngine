//
// Created by micha on 03.04.2023.
//
#include "../include/Objects/DebugShape.h"

#include <vector>

GLuint DebugShape::VAO;
GLuint DebugShape::VBO;
GLuint DebugShape::EBO;
Shader DebugShape::shader;
Model DebugShape::spotModel;

void DebugShape::Initialize()
{
    if (VAO != 0)
        return;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*) 0);

    glBindVertexArray(0);

    shader = Shader("../../res/shaders/debug_shader.vert", "../../res/shaders/debug_shader.frag");
    spotModel = Model((char *)"../../res/models/spot/spot.obj");
}

void DebugShape::DrawCube(glm::vec3 min, glm::vec3 max)
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    std::vector<GLfloat> vertices = {
            max.x, max.y, max.z,
            max.x, min.y, max.z,
            max.x, min.y, min.z,
            max.x, max.y, min.z,

            min.x,  max.y, max.z,
            min.x,  min.y, max.z,
            min.x,  min.y, min.z,
            min.x,  max.y, min.z,
    };
    // Don't sue me
    std::vector<GLuint> indices = {
            0, 1, 2,
            1, 2, 3,

            0, 1, 4,
            1, 4, 5,

            1, 2, 5,
            2, 5, 6,

            2, 3, 6,
            3, 6, 7,

            3, 0, 7,
            0, 7, 4,

            4, 5, 6,
            5, 6, 7
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_DYNAMIC_DRAW);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void DebugShape::DrawArrow(glm::vec3 Start, glm::vec3 End, glm::vec4 Color, glm::mat4 proview)
{
    glm::vec3 Direction = glm::normalize(End - Start);

    glm::vec3 TipOffset = -Direction * 0.5f;
    glm::vec3 TipLeft = End - glm::cross(Direction, glm::vec3(0.f, 0.5f, 0.f)) + TipOffset;
    glm::vec3 TipRight = End + glm::cross(Direction, glm::vec3(0.f, 0.5f, 0.f)) + TipOffset;

    std::vector<glm::vec3> Vertices = {Start, End, TipLeft, TipRight};
    std::vector<GLuint> Indices = {0, 1, 2, 3, 1, 0};

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec3), &Vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(GLuint), &Indices[0], GL_DYNAMIC_DRAW);

    shader.use();
    shader.setVec4("model", Color);
    shader.setMat4("projectionView", proview);

    glDrawElements(GL_LINE_STRIP, Indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}


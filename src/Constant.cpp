//
// Created by tomek on 17.04.2023.
//

#include "../include/Constant.h"
#include "glm/fwd.hpp"
#include "glm/detail/type_mat4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "../../include/Shader.h"

Shader animationShader("animationVertexShaderSource", "animationFragmentShaderSource");

GLuint animationVAO, animationVBO, animationTexture;
float animationTime;

void Constant::initConstant() {
    // Set up vertex data for the animated element (e.g., a simple quad)
    float animationVertices[] = {
            // Positions     // Texture coords
            0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &animationVAO);
    glGenBuffers(1, &animationVBO);

    glBindVertexArray(animationVAO);
    glBindBuffer(GL_ARRAY_BUFFER, animationVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(animationVertices), animationVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set up animation shader

}

void renderConstant() {
    // Render the animated element here
    animationShader.use();

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(600));
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.f, 550.0f, 1.0f));
    model = glm::scale(model, glm::vec3(100.0f, 30.0f, 1.0f));

    animationShader.setMat4("projection", projection);
    animationShader.setMat4("model", model);
    animationShader.setFloat("time", 0.0);
    animationShader.setVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

    glBindVertexArray(animationVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glUseProgram(0);
}

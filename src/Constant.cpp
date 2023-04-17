//
// Created by tomek on 17.04.2023.
//

#include "../include/Constant.h"
#include "glm/fwd.hpp"
#include "glm/detail/type_mat4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

GLuint animationShaderProgram;
GLuint animationVAO, animationVBO, animationTexture;
float animationTime;

GLuint compileShader(const char *source, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Error: Shader compilation failed\n" << infoLog << std::endl;
    }
    return shader;
}

void Constant::initAnimation() {
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
    const char *animationVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;
uniform mat4 projection;
uniform mat4 model;
void main()
{
    gl_Position = projection * model * vec4(aPos, 1.0f);
    TexCoord = aTexCoord;
}
        )";

    const char *animationFragmentShaderSource = R"(
#version 330 core
in vec2 TexCoord;
uniform float time;
uniform vec4 color;
out vec4 FragColor;
void main()
{
    float pulse = abs(sin(time * 2.0f));
    FragColor = mix(vec4(color.xyz, 0.20), vec4(1.0), pulse);
}
        )";

    GLuint animationVertexShader = compileShader(animationVertexShaderSource, GL_VERTEX_SHADER);
    GLuint animationFragmentShader = compileShader(animationFragmentShaderSource, GL_FRAGMENT_SHADER);

    animationShaderProgram = glCreateProgram();
    glAttachShader(animationShaderProgram, animationVertexShader);
    glAttachShader(animationShaderProgram, animationFragmentShader);
    glLinkProgram(animationShaderProgram);

    GLint success;
    glGetProgramiv(animationShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(animationShaderProgram, 512, nullptr, infoLog);
        std::cerr << "Animation shader program linking failed:\n" << infoLog << std::endl;
    }

    glDeleteShader(animationVertexShader);
    glDeleteShader(animationFragmentShader);

}

void renderConstant() {
    // Render the animated element here
    glUseProgram(animationShaderProgram);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(600));
    glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.f, 550.0f, 1.0f));
        model = glm::scale(model, glm::vec3(100.0f, 30.0f, 1.0f));


    glUniformMatrix4fv(glGetUniformLocation(animationShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(animationShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform1f(glGetUniformLocation(animationShaderProgram, "time"), 0.0);
    glUniform4f(glGetUniformLocation(animationShaderProgram, "color"), 0.0f, 0.0f, 0.0f, 0.0f);

    glBindVertexArray(animationVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glUseProgram(0);
}
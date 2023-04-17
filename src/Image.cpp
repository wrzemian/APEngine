//
// Created by tomek on 17.04.2023.
//
#include <iostream>
#include <map>
#include "glm/fwd.hpp"
#include "glm/detail/type_vec4.hpp"
#include "freetype/freetype.h"
#include "glm/detail/type_mat4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <string>
#include "glad/glad.h"
#include "glm/vec3.hpp"
#include "glm/fwd.hpp"
#include "glm/detail/type_vec2.hpp"
#include "../include/Image.h"
#include "stb_image.h"

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

void Image::initImage(const char *path) {
        // Load image using stb_image
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *imageData = stbi_load(path, &width, &height, &channels, 0);
        if (!imageData) {
            std::cerr << "Failed to load image" << std::endl;
            return;
        }

        glGenTextures(1, &imageTexture);
        glBindTexture(GL_TEXTURE_2D, imageTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum format;
        if (channels == 4) {
            format = GL_RGBA;
        } else if (channels == 3) {
            format = GL_RGB;
        } else {
            std::cerr << "Unsupported image format" << std::endl;
            stbi_image_free(imageData);
            return;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(imageData);
        glBindTexture(GL_TEXTURE_2D, 0);

        float imageVertices[] = {
                // Positions     // Texture coords
                0.0f, 1.0f, -1.0f, 0.0f, 1.0f,
                1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
                0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

                0.0f, 1.0f, -1.0f, 0.0f, 1.0f,
                1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
                1.0f, 0.0f, -1.0f, 1.0f, 0.0f
        };

        glGenVertexArrays(1, &imageVAO);
        glGenBuffers(1, &imageVBO);

        glBindVertexArray(imageVAO);
        glBindBuffer(GL_ARRAY_BUFFER, imageVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(imageVertices), imageVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Set up image shader
        const char *imageVertexShaderSource = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec2 aTexCoord;
            out vec2 TexCoord;
            uniform mat4 model;
            uniform mat4 projection;
            void main() {
                gl_Position = projection * model * vec4(aPos, 1.0);
                TexCoord = aTexCoord;
            }
        )";

        const char *imageFragmentShaderSource = R"(
            #version 330 core
            in vec2 TexCoord;
            out vec4 FragColor;
            uniform sampler2D imageTexture;
            void main() {
                FragColor = texture(imageTexture, TexCoord);
            }
        )";

        GLuint imageVertexShader = compileShader(imageVertexShaderSource, GL_VERTEX_SHADER);
        GLuint imageFragmentShader = compileShader(imageFragmentShaderSource, GL_FRAGMENT_SHADER);

        imageShaderProgram = glCreateProgram();
        glAttachShader(imageShaderProgram, imageVertexShader);
        glAttachShader(imageShaderProgram, imageFragmentShader);
        glLinkProgram(imageShaderProgram);

        GLint success;
        glGetProgramiv(imageShaderProgram, GL_LINK_STATUS, &success);
//        if (!success) {
//
//            glGetProgramInfoLog(imageShaderProgram, 512, nullptr, infoLog);
//            std::cerr << "Image shader program linking failed:\n" << infoLog << std::endl;
//        }

        glDeleteShader(imageVertexShader);
        glDeleteShader(imageFragmentShader);
}

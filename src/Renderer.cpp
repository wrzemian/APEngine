//
// Created by tomek on 26.03.2023.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "../include/Engine.h"

class Renderer {
private:
    float pyramid[12 * 5] = {
            // positions                                        // texture coords
            -1.0f, 0.0f, static_cast<float>(-1.0f / sqrt(3)),   1.0f, 1.0f,
            1.0f,  0.0f, static_cast<float>(-1.0f / sqrt(3)),   0.0f, 1.0f,
            0.0f,  static_cast<float>(4.0f / sqrt(6)), 0.0f,    0.5f, 0.0f,

            -1.0f, 0.0f, static_cast<float>(-1.0f / sqrt(3)),   1.0f, 1.0f,
            1.0f,  0.0f, static_cast<float>(-1.0f / sqrt(3)),   0.0f, 1.0f,
            0.0f,  0.0f, static_cast<float>(2.0f / sqrt(3)),    0.5f, 0.0f,

            1.0f,  0.0f, static_cast<float>(-1.0f / sqrt(3)),   1.0f, 1.0f,
            0.0f,  0.0f, static_cast<float>(2.0f / sqrt(3)),    0.0f, 1.0f,
            0.0f,  static_cast<float>(4.0f / sqrt(6)), 0.0f,    0.5f, 0.0f,

            -1.0f, 0.0f, static_cast<float>(-1.0f / sqrt(3)),   1.0f, 1.0f,
            0.0f,  0.0f, static_cast<float>(2.0f / sqrt(3)),    0.0f, 1.0f,
            0.0f,  static_cast<float>(4.0f / sqrt(6)), 0.0f,    0.5f, 0.0f
    };
    unsigned int VBO = 0, VAO = 0, texture = 0;

public:
    void print() {
        std::cout << "Renderer print \n";
    }

    void loadBuffers() {
//        Engine::checkInit();
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid), pyramid, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

    }

    void deleteBuffers() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void loadTexture(const char* path) {
        // load and create a texture
        // -------------------------
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_set_flip_vertically_on_load(true);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
        unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }
};

#endif
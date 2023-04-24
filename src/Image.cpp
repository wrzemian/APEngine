#include <iostream>
#include <map>
#include "glad/glad.h"
#include "../include/Image.h"
#include "stb_image.h"
#include "../include/Shader.h"

Shader shader("res/shaders/imageShader.vert","res/shaders/imageShader.frag",nullptr);

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

    // Use shader program
    shader.use();
    glBindTexture(GL_TEXTURE_2D, imageTexture);
    glBindVertexArray(imageVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);


}
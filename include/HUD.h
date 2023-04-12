//#include <GL/glgw.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//
//#include <ft2build.h>
//#include FT_FREETYPE_H

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <map>
#include <string>
#include <vector>

// Shader utility function
GLuint compileShader(const GLchar *source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

class HUD {
public:
    HUD() {

    }

// Constructor
//    HUD(GLFWwindow *window) : window(window) {
//        init();
//    }

    // Destructor
    ~HUD() {
        // Clean up resources here
    }

    // Public methods
    void render(float deltaTime) {
        // Render your HUD elements here
        renderImage();
        renderText();
        renderAnimation(deltaTime);
    }

    void updateText(const std::string &newText) {
        text = newText;
    }

    void initImage(const char* path) {
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

        // Set up vertex data for image
        float imageVertices[] = {
                // Positions     // Texture coords
                0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, 0.0f, 0.0f, 1.0f, 0.0f
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

    void initAnimation() {
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
            uniform mat4 model;
            uniform mat4 projection;
            void main() {
                gl_Position = projection * model * vec4(aPos, 1.0);
                TexCoord = aTexCoord;
            }
        )";

        const char *animationFragmentShaderSource = R"(
#version 330 core
        in vec2 TexCoord;
        out vec4 FragColor;
        uniform sampler2D animationTexture;
        uniform float time;
        void main() {
            vec4 color = texture(animationTexture, TexCoord);
            // Apply a simple animation effect using time
            color.rgb *= (sin(time
                * 2.0) + 1.0) / 2.0;
                FragColor = color;
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

    void renderImage() {
        // Render the image here
        glUseProgram(imageShaderProgram);

        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(600));
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(200.0f, 200.0f, 1.0f));

        glUniformMatrix4fv(glGetUniformLocation(imageShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(imageShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(imageVAO);
        glBindTexture(GL_TEXTURE_2D, imageTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        glUseProgram(0);
    }

    void renderAnimation(float time) {
        // Render the animated element here
        glUseProgram(animationShaderProgram);

        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(600));
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(300.0f, 300.0f, 1.0f));
        model = glm::scale(model, glm::vec3(1000.0f, 1000.0f, 1.0f));

        glUniformMatrix4fv(glGetUniformLocation(animationShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(animationShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1f(glGetUniformLocation(animationShaderProgram, "time"), time);

        glBindVertexArray(animationVAO);
        glBindTexture(GL_TEXTURE_2D, animationTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        glUseProgram(0);
    }

private:
    // Private members
    GLFWwindow *window;
    GLuint imageShaderProgram;
    GLuint textShaderProgram;
    GLuint animationShaderProgram;
    GLuint imageVAO, imageVBO, imageTexture;
    GLuint textVAO, textVBO;
    GLuint animationVAO, animationVBO, animationTexture;
    float animationTime;
    std::string text;

    // Character structure for FreeType
    struct Character {
        GLuint TextureID;
        glm::ivec2 Size;
        glm::ivec2 Bearing;
        GLuint Advance;
    };
    std::map<GLchar, Character> characters;

    // Private methods
//    void init() {
//        // Initialize HUD components here
//        initImage();
//        initText();
//        initAnimation();
//    }


    void renderText() {
        // Render the text here
        glUseProgram(textShaderProgram);

        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(600));
        glUniformMatrix4fv(glGetUniformLocation(textShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3f(glGetUniformLocation(textShaderProgram, "textColor"), 1.0f, 1.0f, 1.0f);

        float x = 50.0f;
        float y = 500.0f;
        float scale = 1.0f;

        glBindVertexArray(textVAO);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(textShaderProgram, "textTexture"), 0);

        for (auto c : text) {
            Character ch = characters[c];

            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;

            float vertices[6][4] = {
                    { xpos,     ypos + h, 0.0f, 0.0f },
                    { xpos,     ypos,     0.0f, 1.0f },
                    { xpos + w, ypos,     1.0f, 1.0f },

                    { xpos,  ypos + h, 0.0f, 0.0f },
                    { xpos + w, ypos,     1.0f, 1.0f },
                    { xpos + w, ypos + h, 1.0f, 0.0f }
            };

            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            glBindBuffer(GL_ARRAY_BUFFER, textVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            x += (ch.Advance >> 6) * scale;
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
    }

};



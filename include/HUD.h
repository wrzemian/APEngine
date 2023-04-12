//#include <GL/glgw.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <map>
#include <string>
#include <vector>

bool isVisable = false;


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

    void renderImage(float x) {
        // Render the image here
        glUseProgram(imageShaderProgram);

        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(600));
        glm::mat4 model = glm::mat4(1.0f);
        if(isVisable == true) {
            model = glm::translate(model, glm::vec3(x * 1.0f, -100.0f, 1.0f));
            model = glm::scale(model, glm::vec3(5000.0f, 1000.0f, 1.0f));
        }
        else{
            model = glm::translate(model, glm::vec3(-10000, -100.0f, 4.0f));
            model = glm::scale(model, glm::vec3(5000.0f, 1000.0f, 1.0f));
        }

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
        if(isVisable == true && time * 2.f < 100.0f) {
            model = glm::translate(model, glm::vec3(2.f, 520.0f, 1.0f));
            model = glm::scale(model, glm::vec3(100.0f - time * 2.f, 30.0f, 1.0f));
        }
        else{
            model = glm::translate(model, glm::vec3(-10000, -100.0f, 4.0f));
            model = glm::scale(model, glm::vec3(5000.0f, 1000.0f, 1.0f));
        }


        glUniformMatrix4fv(glGetUniformLocation(animationShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(animationShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1f(glGetUniformLocation(animationShaderProgram, "time"), time);
        glUniform4f(glGetUniformLocation(animationShaderProgram, "color"), 1.0f, 0.0f, 0.0f, 1.0f);

        glBindVertexArray(animationVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glUseProgram(0);
    }

    void renderConstant() {
        // Render the animated element here
        glUseProgram(animationShaderProgram);

        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(600));
        glm::mat4 model = glm::mat4(1.0f);
        if(isVisable == true) {
            model = glm::translate(model, glm::vec3(2.f, 550.0f, 1.0f));
            model = glm::scale(model, glm::vec3(100.0f, 30.0f, 1.0f));
        }
        else{
            model = glm::translate(model, glm::vec3(-10000, -100.0f, 4.0f));
            model = glm::scale(model, glm::vec3(5000.0f, 1000.0f, 1.0f));
        }


        glUniformMatrix4fv(glGetUniformLocation(animationShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(animationShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1f(glGetUniformLocation(animationShaderProgram, "time"), 0.0);
        glUniform4f(glGetUniformLocation(animationShaderProgram, "color"), 0.0f, 0.0f, 0.0f, 0.0f);

        glBindVertexArray(animationVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glUseProgram(0);
    }


    void ImGui(){

        ImGui::Begin("hud");
        ImGui::SetWindowSize(ImVec2(100, 50));

        ImGui::Checkbox("show hud", &isVisable);

        ImGui::End();
    }

private:
    // Private members
    GLFWwindow *window;
    GLuint imageShaderProgram;
    GLuint animationShaderProgram;
    GLuint imageVAO, imageVBO, imageTexture;
    GLuint animationVAO, animationVBO, animationTexture;
    float animationTime;
    std::string text;

};



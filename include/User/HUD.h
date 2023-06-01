#ifndef HUD_H
#define HUD_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H
#include "../IGui.h"
#include "../Shader.h"

#include "glad/glad.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

class HUD : public IGui {
public:
    bool isVisable = false;

    void initImage(const char* path);
    void updateText(const std::string& newText);
    void renderImage(float x);
    void ImGui();

    void initText(const std::string& fontPath);
    void renderText(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, const glm::vec3& color);

    HUD() = default; // TODO: fix defeault constructor
    explicit HUD(Shader shader, Shader textShader);
    explicit HUD(Shader shader);

    ~HUD();

private:
    struct Character {
        GLuint textureID;
        glm::ivec2 size;
        glm::ivec2 bearing;
        GLuint advance;
    };

    Shader shader, textShader;
    GLFWwindow* window;
    GLuint animationShaderProgram;
    GLuint imageShaderProgram;
    GLuint imageVAO, imageVBO, imageTexture;
    GLuint animationVAO, animationVBO, animationTexture;
    float animationTime;
    std::string text;
    GLuint textShaderProgram;
    GLuint fontTexture;
    GLint projectionUniformLocation;
    GLint textColorUniformLocation;
    glm::mat4 projectionMatrix;
    std::map<char, glm::ivec4> charData;

    void loadCharacters();
    void createShaderProgram();
    GLuint compileShader(const char* source, GLenum shaderType);

    FT_Library library;
    FT_Face face;
    GLuint shaderProgram;
    GLuint vao, vbo;
    std::map<char, Character> characters;
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(600));
};

#endif // HUD_H

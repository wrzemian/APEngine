#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <ft2build.h>
#include FT_FREETYPE_H
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
    struct Character {
        GLuint textureID;
        glm::ivec2 size;
        glm::ivec2 bearing;
        GLuint advance;
    };

    HUD() {

    }

    ~HUD() {
    }


    void updateText(const std::string &newText);

    void initImage(const char *path);


    void initAnimation();

    void renderAnimation(float time);

    void renderConstant();

    void imguiText();


    void ImGui();

private:
    // Private members
    GLFWwindow *window;
    GLuint imageShaderProgram;
    GLuint animationShaderProgram;
    GLuint imageVAO, imageVBO, imageTexture;
    GLuint animationVAO, animationVBO, animationTexture;
    float animationTime;



    //GLuint vao, vbo;
    GLint projectionUniformLocation;

    glm::mat4 projectionMatrix;
    std::map<char, glm::ivec4> charData;

    void loadCharacters();

    void createShaderProgram();

    GLuint compileShader(const char *source, GLenum shaderType);
};
//#include <GL/glgw.h>
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

bool isVisable = true;


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

// Constructor
//    HUD(GLFWwindow *window) : window(window) {
//        init();
//    }

    // Destructor
    ~HUD() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteTextures(1, &fontTexture);
        glDeleteProgram(textShaderProgram);
        // Clean up resources here
    }


    void updateText(const std::string &newText) {
        text = newText;
    }
/*
    void initCharData() {
        const int gridSize =16;
        const int charWidth = 32;
        const int charHeight = 32;

        for (int i = 0; i < gridSize; ++i) {
            for (int j = 0; j < gridSize; ++j) {
                char c = static_cast<char>(32 + i * gridSize + j);
                int x = j * charWidth;
                int y = i * charHeight;
                charData[c] = glm::ivec4(x, y, charWidth, charHeight);
            }
        }
    }
    */
/*
    void load(const std::string& fontTexturePath) {
        initCharData();
        projectionMatrix = glm::ortho(0.0f, static_cast<float>(1000), 0.0f, static_cast<float>(800));
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenTextures(1, &fontTexture);
        glBindTexture(GL_TEXTURE_2D, fontTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(fontTexturePath.c_str(), &width, &height, &channels, 0);
        if (data) {
            if (channels == 3) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            } else if (channels == 4) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            }
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load font texture: " << fontTexturePath << std::endl;
        }
        stbi_image_free(data);

        // Set up image shader
        const char *textVertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec4 vertex;
        out vec2 TexCoords;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
            TexCoords = vertex.zw;
        }
        )";

        const char *textFragmentShaderSource = R"(
            #version 330 core
            in vec2 TexCoords;
            out vec4 color;
            uniform sampler2D text;
            uniform vec3 textColor;
            void main() {
                vec4 sampled = texture(text, TexCoords);
                color = vec4(textColor, 1.0) * sampled;
            }
        )";

        GLuint textVertexShader = compileShader(textVertexShaderSource, GL_VERTEX_SHADER);
        GLuint textFragmentShader = compileShader(textFragmentShaderSource, GL_FRAGMENT_SHADER);

        textShaderProgram = glCreateProgram();
        glAttachShader(textShaderProgram, textVertexShader);
        glAttachShader(textShaderProgram, textFragmentShader);
        glLinkProgram(textShaderProgram);

        textColorUniformLocation = glGetUniformLocation(textShaderProgram, "textColor");
        projectionUniformLocation = glGetUniformLocation(textShaderProgram, "projection");

        GLint success;
        glGetProgramiv(textShaderProgram, GL_LINK_STATUS, &success);

        glDeleteShader(textVertexShader);
        glDeleteShader(textFragmentShader);
    }
*/
   /* void renderText(const std::string& text, float x, float y, float scale, const glm::vec3& color) {
        glUseProgram(textShaderProgram);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fontTexture);

        glUniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(textColorUniformLocation, color.x, color.y, color.z);

        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) {
            glm::ivec4 ch = charData[*c];

            GLfloat xpos = x + ch.z * scale;
            GLfloat ypos = y - (ch.w - ch.y) * scale;

            GLfloat w = ch.z * scale;
            GLfloat h = ch.w * scale;

            GLfloat vertices[] = {
                    xpos, ypos + h, (float)ch.x, (float)ch.y,
                    xpos + w, ypos, (float)ch.x + (float)ch.z, (float)ch.y + (float)ch.w,
                    xpos, ypos, (float)ch.x, (float)ch.y + (float)ch.w,

                    xpos, ypos + h, (float)ch.x, (float)ch.y,
                    xpos + w, ypos + h, (float)ch.x + (float)ch.z, (float)ch.y,
                    xpos + w, ypos, (float)ch.x + (float)ch.z, (float)ch.y + (float)ch.w
            };

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *) 0);
            glEnableVertexAttribArray(0);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            x += (ch.z + 1) * scale; // Advance the cursor to the next glyph
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(0);
    }
*/










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

    void renderImage(float x) {
        // Render the image here
        glUseProgram(imageShaderProgram);

        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(600));
        glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(x * 1.0f, -100.0f, 1.0f));
            model = glm::scale(model, glm::vec3(5000.0f, 1000.0f, 1.0f));

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

    void imguiText(){

    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGui::Begin("Transparent Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

    ImVec4 textColor(0.0f, 0.0f, 0.0f, 1.0f);
    ImGuiCol textColorID = ImGuiCol_Text;

    ImGui::PushStyleColor(textColorID, textColor);
    ImGui::Text("FPS: %f", 1.0f / Engine::deltaTime);

    ImGui::PopStyleColor();
    ImGui::End();

    }


    void ImGui(){

        ImGui::Begin("hud");
        ImGui::SetWindowSize(ImVec2(130, 50));

        ImGui::Checkbox("show hud", &isVisable);

        ImGui::End();
    }

    void initText(const std::string &fontPath) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        if (FT_Init_FreeType(&library)) {
            std::cerr << "Error: Could not initialize FreeType library" << std::endl;
            return;
        }

        if (FT_New_Face(library, fontPath.c_str(), 0, &face)) {
            std::cerr << "Error: Failed to load font: here " << fontPath << std::endl;
            return;
        }

        FT_Set_Pixel_Sizes(face, 0, 48);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        loadCharacters();
        createShaderProgram();
    }

    void renderText(const std::string &text, GLfloat x, GLfloat y, GLfloat scale, const glm::vec3 &color) {
        glUseProgram(shaderProgram);
        glUniform3f(glGetUniformLocation(shaderProgram, "textColor"), color.x, color.y, color.z);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(vao);

        for (const char &c : text) {
            Character ch = characters[c];
            GLfloat xpos = x + ch.bearing.x * scale;
            GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;
            GLfloat w = ch.size.x * scale;
            GLfloat h = ch.size.y * scale;
            GLfloat vertices[6][4] = {
                    { xpos,     ypos + h,   0.0, 0.0 },
                    { xpos,     ypos,       0.0, 1.0 },
                    { xpos + w, ypos,       1.0, 1.0 },
                    { xpos,     ypos + h,   0.0, 0.0 },
                    { xpos + w, ypos,       1.0, 1.0 },
                    { xpos + w, ypos + h,   1.0, 0.0 }
            };
            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            x += (ch.advance >> 6) * scale;
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
    }

private:
    // Private members
    GLFWwindow *window;
    GLuint animationShaderProgram;

    GLuint imageShaderProgram;
    GLuint imageVAO, imageVBO, imageTexture;
    GLuint animationVAO, animationVBO, animationTexture;
    float animationTime;
    std::string text;

    GLuint textShaderProgram;
    GLuint fontTexture;
    //GLuint vao, vbo;
    GLint projectionUniformLocation;
    GLint textColorUniformLocation;
    glm::mat4 projectionMatrix;
    std::map<char, glm::ivec4> charData;
    void loadCharacters() {
        for (GLubyte c = 0; c < 128; c++) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cerr << "Error: Failed to load glyph for character: " << c << std::endl;
                continue;
            }
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            Character character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<GLuint>(face->glyph->advance.x)
            };
            characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void createShaderProgram() {
        const GLchar *vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec4 vertex;
        out vec2 TexCoords;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
            TexCoords = vertex.zw;
        }
    )";

        const GLchar *fragmentShaderSource = R"(
        #version 330 core
        in vec2 TexCoords;
        out vec4 color;
        uniform sampler2D text;
        uniform vec3 textColor;
        void main() {
            vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
            color = vec4(textColor, 1.0) * sampled;
        }
    )";

        GLuint vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
        GLuint fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

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

    FT_Library library;
    FT_Face face;
    GLuint shaderProgram;
    GLuint vao, vbo;
    std::map<char, Character> characters;
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(600));
};



////#include "../include/Text.h"
//
//
//GLuint textShaderProgram;
//GLuint fontTexture;
////GLuint vao, vbo;
//glm::mat4 projectionMatrix;
//GLint projectionUniformLocation;
//GLint textColorUniformLocation;
//FT_Library library;
//FT_Face face;
//GLuint shaderProgram;
//GLuint vao, vbo;
//std::map<char, glm::ivec4> charData;
//
//struct Character {
//    GLuint textureID;
//    glm::ivec2 size;
//    glm::ivec2 bearing;
//    GLuint advance;
//};
//
//std::map<char, Character> characters;
//glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(600));
//
//
//
//
//void loadCharacters() {
//    for (GLubyte c = 0; c < 128; c++) {
//        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
//            std::cerr << "Error: Failed to load glyph for character: " << c << std::endl;
//            continue;
//        }
//        GLuint texture;
//        glGenTextures(1, &texture);
//        glBindTexture(GL_TEXTURE_2D, texture);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        Character character = {
//                texture,
//                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
//                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
//                static_cast<GLuint>(face->glyph->advance.x)
//        };
//        characters.insert(std::pair<char, Character>(c, character));
//    }
//    glBindTexture(GL_TEXTURE_2D, 0);
//}
//
//
//GLuint compileShader(const char *source, GLenum shaderType) {
//    GLuint shader = glCreateShader(shaderType);
//    glShaderSource(shader, 1, &source, nullptr);
//    glCompileShader(shader);
//    GLint success;
//    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//    if (!success) {
//        GLchar infoLog[512];
//        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
//        std::cerr << "Error: Shader compilation failed\n" << infoLog << std::endl;
//    }
//    return shader;
//}
//
//void createShaderProgram() {
//    const GLchar *vertexShaderSource = R"(
//        #version 330 core
//        layout (location = 0) in vec4 vertex;
//        out vec2 TexCoords;
//        uniform mat4 projection;
//        void main() {
//            gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
//            TexCoords = vertex.zw;
//        }
//    )";
//    const GLchar *fragmentShaderSource = R"(
//        #version 330 core
//        in vec2 TexCoords;
//        out vec4 color;
//        uniform sampler2D text;
//        uniform vec3 textColor;
//        void main() {
//            vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
//            color = vec4(textColor, 1.0) * sampled;
//        }
//    )";
//
//    GLuint vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
//    GLuint fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
//    shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//}
//
//
//void Text::initText(const std::string &fontPath) {
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glGenVertexArrays(1, &vao);
//    glGenBuffers(1, &vbo);
//    glBindVertexArray(vao);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//
//    if (FT_Init_FreeType(&library)) {
//        std::cerr << "Error: Could not initialize FreeType library" << std::endl;
//        return;
//    }
//
//    if (FT_New_Face(library, fontPath.c_str(), 0, &face)) {
//        std::cerr << "Error: Failed to load font: here " << fontPath << std::endl;
//        return;
//    }
//
//    FT_Set_Pixel_Sizes(face, 0, 48);
//
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//    loadCharacters();
//    createShaderProgram();
//
//}
//
//
//void Text::renderText(const std::string &text, GLfloat x, GLfloat y, GLfloat scale, const glm::vec3 &color) {
//    glUseProgram(textShaderProgram);
//    glBindVertexArray(vao);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, fontTexture);
//    glUniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
//    glUniform3f(textColorUniformLocation, color.x, color.y, color.z);
//    std::string::const_iterator c;
//    for (c = text.begin(); c != text.end(); c++) {
//        glm::ivec4 ch = charData[*c];
//        GLfloat xpos = x + ch.z * scale;
//        GLfloat ypos = y - (ch.w - ch.y) * scale;
//        GLfloat w = ch.z * scale;
//        GLfloat h = ch.w * scale;
//        GLfloat vertices[] = {
//                xpos, ypos + h, (float)ch.x, (float)ch.y,
//                xpos + w, ypos, (float)ch.x + (float)ch.z, (float)ch.y + (float)ch.w,
//                xpos, ypos, (float)ch.x, (float)ch.y + (float)ch.w,
//                xpos, ypos + h, (float)ch.x, (float)ch.y,
//                xpos + w, ypos + h, (float)ch.x + (float)ch.z, (float)ch.y,
//                xpos + w, ypos, (float)ch.x + (float)ch.z, (float)ch.y + (float)ch.w
//        };
//        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
//        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *) 0);
//        glEnableVertexAttribArray(0);
//        glDrawArrays(GL_TRIANGLES, 0, 6);
//        x += (ch.z + 1) * scale; // Advance the cursor to the next glyph
//    }
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//    glUseProgram(0);
//}
//
//
//
//

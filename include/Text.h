//
// Created by tomek on 17.04.2023.
//

#ifndef APENGINE_TEXT_H
#define APENGINE_TEXT_H

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

class Text {
private :

    std::string text;
    GLuint textShaderProgram;
    GLuint fontTexture;
    GLint textColorUniformLocation;

public:

    void initText(const std::string &fontPath);

    void renderText(const std::string &text, GLfloat x, GLfloat y, GLfloat scale, const glm::vec3 &color);
};


#endif //APENGINE_TEXT_H

//
// Created by tomek on 26.04.2023.
//

#ifndef APENGINE_IMAGE_H
#define APENGINE_IMAGE_H


#include <iostream>
#include "stb_image.h"
#include "glad/glad.h"
//#include "glm/ext/matrix_clip_space.hpp"
#include "glm/fwd.hpp"
//#include "glm/detail/type_mat3x2.hpp"
//#include "glm/gtc/type_ptr.hpp"
#include "Shader.h"

class Image {
private:
    GLuint imageShaderProgram;
    GLuint imageVAO, imageVBO, imageTexture;
    Shader shader;
public:
    Image(Shader imageShader);

    Image();

    void initImage(const char* path);

    void renderImage(float x);
};



#endif //APENGINE_IMAGE_H

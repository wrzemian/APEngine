//
// Created by tomek on 17.04.2023.
//

#ifndef APENGINE_IMAGE_H
#define APENGINE_IMAGE_H


class Image {
private:
    GLuint imageShaderProgram;
    GLuint imageVAO, imageVBO, imageTexture;
public:
    void initImage(const char *path);
};


#endif //APENGINE_IMAGE_H

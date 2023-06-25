//
// Created by tomek on 25.06.2023.
//

#ifndef APENGINE_SPRITERENDERER_H
#define APENGINE_SPRITERENDERER_H


#include "../Shader.h"

class SpriteRenderer {
public:
    bool isVisable = true;
    SpriteRenderer();
    SpriteRenderer(Shader &shader);
    ~SpriteRenderer();
    void DrawSprite(glm::vec3 position, glm::vec2 size, float rotate);
    void initRenderData(const char *path);
    void renderUI();

private:

    GLuint imageVAO, imageVBO, imageTexture;
    Shader shader;
    unsigned int quadVAO;
    int counter = 0;

};


#endif //APENGINE_SPRITERENDERER_H

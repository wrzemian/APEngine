#ifndef APENGINE_MENU_H
#define APENGINE_MENU_H


#include "glad/glad.h"
#include "../Shader.h"
#include "InputSystem.h"
#include "SpriteRenderer.h"

class Menu
        {
private:
    GLuint imageVAO, imageVBO, imageTexture;
    Shader shader;
    float padJoystickTolerance = 0.2f;
    SpriteRenderer mark[3];
    glm::vec3 markPosHelper;
    bool joystickMoved = false;
    bool joystickMoved2 = false;
    bool isPause = false;
    bool isEnd = false;
    int counter;


public:
    bool isVisible1() const;
    Menu(const Shader &shader);
    Menu();

    Menu(const Shader &shader, bool isPause);

    virtual ~Menu();

    void pickMenu(int i);
    void processInput(InputSystem* inputSystem);
    void initMenu(const char *path);
    void drawMenu(glm::vec3 position,
                  glm::vec2 size, float rotate);


    bool isVisible = true;

    glm::vec3 markPos[3];
    glm::vec2 markScale[3];
};

#endif //APENGINE_MENU_H

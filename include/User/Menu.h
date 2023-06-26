#ifndef APENGINE_MENU_H
#define APENGINE_MENU_H


#include "glad/glad.h"
#include "../Shader.h"
#include "InputSystem.h"

class Menu {
private:
    GLuint imageVAO, imageVBO, imageTexture;
    Shader shader;


private:
    float padJoystickTolerance = 0.5f;

public:
    bool isVisible1() const;
    Menu(const Shader &shader);
    Menu();

    virtual ~Menu();

    void pickMenu(int i);
    void processInput(InputSystem* inputSystem);
    void initMenu(const char *path);
    void drawMenu(glm::vec3 position,
                  glm::vec2 size, float rotate);

    int counter = 0;
    bool isVisible = true;
};

#endif //APENGINE_MENU_H

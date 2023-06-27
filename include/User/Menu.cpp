//
// Created by tomek on 26.06.2023.
//

#include "Menu.h"
#include "stb_image.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "InputSystem.h"

void Menu::pickMenu(int chosen) {
    switch (chosen) {
        case 0:
            isVisible = false;
            break;

        case 1:
            if(isPause == true){
                Game::ResetLevel();
                isVisible = false;
            }
            else {
                isVisible = false;
            }
            break;

        case 2:
            glfwSetWindowShouldClose(Engine::getWindow(),GLFW_TRUE);
            break;

    }
}

void Menu::processInput(InputSystem* inputSystem) {
    int i;
    if(inputSystem->getJoystickAxis(0, GLFW_GAMEPAD_AXIS_LEFT_X)  < -0.2|| inputSystem->GetGamepadButtonDown(0, GLFW_GAMEPAD_BUTTON_DPAD_LEFT)){
        if(joystickMoved == false) {
            this->counter--;
            joystickMoved = true;
        }
        i = abs(counter%3);
        markPosHelper = markPos[i];
    }
    else if(inputSystem->getJoystickAxis(0, GLFW_GAMEPAD_AXIS_LEFT_X) > 0.2 || inputSystem->GetGamepadButtonDown(0, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT)){
        if(joystickMoved == false) {
            this->counter++;
            joystickMoved = true;
        }
        i = abs(counter%3);
        markPosHelper = markPos[i];
    }
    else if (inputSystem->GetKeyDown(GLFW_KEY_LEFT)) {
        this->counter--;
        i = abs(counter%3);
        markPosHelper = markPos[i];
    }
    else if (inputSystem->GetKeyDown(GLFW_KEY_RIGHT)) {
        this->counter++;
        i = abs(counter%3);
        markPosHelper = markPos[i];
    }
    else if(inputSystem->GetKeyDown(GLFW_KEY_SPACE) || inputSystem->GetGamepadButtonDown(0, GLFW_GAMEPAD_BUTTON_A)){
        this->pickMenu(abs(counter%3));
    }
    else if(inputSystem->getJoystickAxis(0, GLFW_GAMEPAD_AXIS_LEFT_Y) < 0.2 && inputSystem->getJoystickAxis(0, GLFW_GAMEPAD_AXIS_LEFT_Y)> -0.2 )
    {
        joystickMoved = false;
    }

}

void Menu::initMenu(const char *filePath) {
    // Load image using stb_image
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *imageData = stbi_load(filePath, &width, &height, &channels, 0);
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

    mark[0].initRenderData("res/UI/start.png");
    mark[1].initRenderData("res/UI/load.png");
    mark[2].initRenderData("res/UI/exit.png");
}

void Menu::drawMenu(glm::vec3 position,
                    glm::vec2 size, float rotate) {
    {
        mark[abs(counter%3)].DrawSprite(markPosHelper,glm::vec2(250.f,130.f),0.f);
        shader.use();

        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(600));
        glm::mat4 model = glm::mat4(1.0f);
        if (isVisible) {
            model = glm::translate(model, position);
            model = glm::scale(model, glm::vec3(size, 1.0f));

            model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
            model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
        } else {

        };

        shader.setMat4("projection", projection);
        shader.setMat4("model", model);

        glBindVertexArray(imageVAO);
        glBindTexture(GL_TEXTURE_2D, imageTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);

    };
}

Menu::Menu(const Shader &shader) : shader(shader) {
    for(int i=0; i<3;i++) {
        SpriteRenderer mark1(this->shader);
        mark[i] = mark1;
    }
    markPos[0] = glm::vec3(20.f,0.f,1.f);
    markPos[1] = glm::vec3(275.f,0.f,1.f);
    markPos[2] = glm::vec3(520.f,0.f,1.f);
    markPosHelper = markPos[0];
    isPause = false;
}

Menu::Menu() {}

Menu::~Menu() {

}

bool Menu::isVisible1() const {
    return isVisible;

}

Menu::Menu(const Shader &shader, bool isPause) : shader(shader), isPause(isPause) {

    for(int i=0; i<3;i++) {
        SpriteRenderer mark1(this->shader);
        mark[i] = mark1;
    }
    markPos[0] = glm::vec3(0.f,0.f,1.f);
    markPos[1] = glm::vec3(275.f,0.f,1.f);
    markPos[2] = glm::vec3(520.f,0.f,1.f);
    markPosHelper = markPos[0];
    isPause = true;
    isVisible = false;
}

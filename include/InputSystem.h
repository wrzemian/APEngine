//
// Created by Karol on 02.04.2023.
//

#ifndef APENGINE_INPUTSYSTEM_H
#define APENGINE_INPUTSYSTEM_H

#include "../include/Engine.h"
#include <unordered_map>
#include <vector>

class InputSystem {
public:
    void InputInit();

    void update();

    void monitorKey(int key);

    bool GetKey(int key);

    bool GetKeyDown(int key);

    bool isGamepadButtonPressed(int gamepadIndex, int button);

    bool GetGamepadButtonDown(int gamepadIndex, int button);

    float getJoystickAxis(int gamepadIndex, int axis);

    float GetAxis(int key);

private:
    GLFWwindow* window;

    std::vector<int> monitoredKeys;
    std::unordered_map<int, bool> keyStates;
    std::unordered_map<int, bool> lastKeyStates;

    std::vector<std::unordered_map<int, bool>> gamepadButtonStates = std::vector<std::unordered_map<int, bool>>(2);
    std::vector<std::unordered_map<int, bool>> lastGamepadButtonStates = std::vector<std::unordered_map<int, bool>>(2);


    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    static void joystick_callback(int jid, int event) {
        if (event == GLFW_CONNECTED) {
            std::cout << "Joystick " << jid << " connected." << std::endl;
        } else if (event == GLFW_DISCONNECTED) {
            std::cout << "Joystick " << jid << " disconnected." << std::endl;
        }
    }
};


#endif //APENGINE_INPUTSYSTEM_H

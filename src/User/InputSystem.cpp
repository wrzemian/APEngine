//
// Created by Karol on 02.04.2023.
//

#include "../../include/User/InputSystem.h"



void InputSystem::InputInit()
{
    window = Engine::window;
    glfwSetKeyCallback(window, key_callback);
    glfwSetJoystickCallback(joystick_callback);
}

void InputSystem::update() {
    lastKeyStates = keyStates;
    keyStates.clear();

    for (const auto& key : monitoredKeys) {
        int state = glfwGetKey(window, key);
        keyStates[key] = state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    for (int gamepadIndex = 0; gamepadIndex < 2; ++gamepadIndex) {
        int joystickId = gamepadIndex == 1 ? GLFW_JOYSTICK_2 : GLFW_JOYSTICK_1;
        lastGamepadButtonStates[gamepadIndex] = gamepadButtonStates[gamepadIndex];
        gamepadButtonStates[gamepadIndex].clear();

        int present = glfwJoystickPresent(joystickId);
        if (present) {
            int count;
            const unsigned char* buttons = glfwGetJoystickButtons(joystickId, &count);
            for (int button = 0; button < count; ++button) {
                gamepadButtonStates[gamepadIndex][button] = buttons[button] == GLFW_PRESS;
            }
        }
    }
}

void InputSystem::monitorKey(int key) {
    monitoredKeys.push_back(key);
}

bool InputSystem::GetKey(int key)
{
    return keyStates[key];
}

bool InputSystem::GetKeyDown(int key) {
    return keyStates[key] && !lastKeyStates[key];
}

bool InputSystem::isGamepadButtonPressed(int gamepadIndex, int button) {
    return gamepadButtonStates[gamepadIndex][button];
}

bool InputSystem::GetGamepadButtonDown(int gamepadIndex, int button) {
    return gamepadButtonStates[gamepadIndex][button] && !lastGamepadButtonStates[gamepadIndex][button];
}

float InputSystem::getJoystickAxis(int gamepadIndex,int axis) {
    int joystickId = gamepadIndex == 1 ? GLFW_JOYSTICK_2 : GLFW_JOYSTICK_1;
    int present = glfwJoystickPresent(joystickId);
    if (present) {
        int count;
        const float* axes = glfwGetJoystickAxes(joystickId, &count);
        return axes[axis];
    }
    return 0.0f;
}

float InputSystem::GetAxis(int key)
{
    GLFWgamepadstate state;
    if (glfwGetGamepadState(GLFW_JOYSTICK_3, &state))
    {
        return state.axes[key];
    }
    return 0;
}
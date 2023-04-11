//
// Created by Karol on 02.04.2023.
//

#include "../include/InputSystem.h"



void InputSystem::InputInit()
{
    window = Engine::getWindow();
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

bool InputSystem::isGamepadButtonPressed(int button) {
    int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
    if (present) {
        int count;
        const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
        return buttons[button] == GLFW_PRESS;
    }
    return false;
}

float InputSystem::getJoystickAxis(int axis) {
    int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
    if (present) {
        int count;
        const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
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
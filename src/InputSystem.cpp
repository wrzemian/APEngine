//
// Created by Karol on 02.04.2023.
//

#include "../include/InputSystem.h"

bool InputSystem::GetKeyDown(GLFWwindow* window, int key)
{
    if (glfwGetKey(window, key) == GLFW_PRESS)
    {
        return true;
    }
    else{
        return false;
    }
}

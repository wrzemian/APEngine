//
// Created by tomek on 26.03.2023.
//
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"

#include <vector>
#include <iostream>

namespace Engine {
    extern double deltaTime;

    void ImGui();
    int Init();
    void LoopStart();
    void LoopEnd();

    void terminate();

    int initGLandImGui();

    void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    void processInput(GLFWwindow* window);

    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void processInput(GLFWwindow* window);

    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    int initGLandImGui();
}

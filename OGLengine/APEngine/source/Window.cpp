#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <iostream>


inline void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}




class Window
{
private:
    // settings
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

    GLFWwindow* _window;

public:
    float lastX = 800 / 2.0f;
    float lastY = 600 / 2.0f;
    bool firstMouse = true;
    bool useMouse = true;

    GLfloat xoffset;
    GLfloat yoffset;

   

    int init()
    {
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        // glfw window creation
        // --------------------
        _window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
        if (_window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(_window);
        glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

    }
   

    void processInput()
    {
        if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(_window, true);
        }
    }

    bool isOpen()
    {
        return !glfwWindowShouldClose(_window);
    }

    void clear()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void swap()
    {
        glfwSwapBuffers(_window);
    }

    void pollEvents()
    {
        glfwPollEvents();

    }

    void terminate()
    {
        glfwTerminate();
    }

    GLFWwindow* getWindow()
    {
        return _window;
    }
};
#endif


#ifndef ENGINE_H
#define ENGINE_H

#include "Triangle.cpp"
#include <glm.hpp>
#include <glad/glad.h>
#include <glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <Window.cpp>




class Engine
{

public:
    Triangle triangle;

    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    void setup()
    {
        //Renderer rnd;

        //rnd.Update();

        window.init();

        triangle.init();
        
    }

    bool isOn()
    {
        return window.isOpen();
    }

    void Update()
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("GENERAL");

            ImGui::End();
        }
        window.processInput();
        window.clear();

        triangle.draw();



        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.swap();
        window.pollEvents();
    }


    void close()
    {
        // glfw: terminate, clearing all previously allocated GLFW resources.
        // ------------------------------------------------------------------
        window.terminate();
        ImGui::DestroyContext();
    }

    Window* getWindow()
    {
        return &window;
    }
private:
    Window window;

};
#endif
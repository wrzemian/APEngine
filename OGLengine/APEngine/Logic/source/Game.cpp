#include "Game.h"
#include "Triangle.h"
#include "GameObject/Component.h"
#include "GameObject/Renderer.h"
#include "HID.h"
#include <glm.hpp>
#include "Camera.h"
#include "globals.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>


Triangle triangle;

HID hid;

float camX = 0.0f;
float camY = 0.0f;
float camZ = 0.0f;
Camera camera(glm::vec3(camX, camY, camZ));

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
//
//float lastX = 800 / 2.0f;
//float lastY = 600 / 2.0f;
//bool firstMouse = true;
//bool useMouse = true;

void Game::setup()
{
    //Renderer rnd;

    //rnd.Update();

    


    window.init();
    
    triangle.init();

   
}

bool Game::isOn()
{
    return window.isOpen();
}

void Game::Update()
{
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        ImGui::Begin("GENERAL");
        ImGui::SliderFloat("X", &camX, -3.0, 3.0);
        ImGui::SliderFloat("Y", &camY, -3.0, 3.0);
        ImGui::SliderFloat("Z", &camZ, -3.0, 3.0);
        ImGui::End();
    }
    camera.Position = glm::vec3(camX, camY, camZ);
    window.processInput();
    window.clear();

    //logic
    
    camera.ProcessMouseMovement(xoffset, yoffset);
    //printf("X: %F, Y: %F \n", xoffset, yoffset);
    //draw objects
    


    triangle.draw(camera);


    
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    window.swap();
    window.pollEvents();
}



void Game::close()
{
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    window.terminate();
    ImGui::DestroyContext();
}

Window* Game::getWindow()
{
    return &window;
}

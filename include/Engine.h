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

#include "IHitbox.h"
#include "IGui.h"
#include "Hitbox.h"
#include "MovingObject.h"
#include "Object3D.h"


namespace Engine {
    extern float deltaTime;

    void ImGui();
    int Init();
    void LoopStart();
    void LoopEnd();

    int getHitboxIndex();
    int getImguiIndex();

    void addHitbox(Hitbox* hitbox);
    void addImgui(IGui* imgui);
    void addMovingObject(MovingObject* object);
    void addObject(Object3D* object);

    void removeHitbox(Hitbox* hitbox);
    void removeImgui(IGui* igui);
    void removeMovingObject(MovingObject* object);
    void removeObject(Object3D* object);



    void renderHitboxes(const glm::mat4& projectionView);
    void renderImgui();

    void moveObjects();
    void drawObjects();

    void terminate();
    void resolveCollisions();


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

    GLFWwindow* getWindow();
}

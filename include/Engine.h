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

#include "IGui.h"
#include "Objects/Hitbox.h"
#include "Objects/MovingObject.h"
#include "Objects/Object3D.h"
#include "Parser.h"
#include "Camera.h"
#include "Objects/Ant.h"


namespace Engine {
    extern float deltaTime;
    extern Parser parser;
    extern int SCR_WIDTH;
    extern int SCR_HEIGHT;
    extern bool frustum;

    void ImGui();
    int Init();
    void LoopStart();
    void LoopEnd();

    int getHitboxIndex();
    int getImguiIndex();
    int getObject3DIndex(std::shared_ptr<Object3D> obj);
    std::shared_ptr<Object3D> getObject3DById(int id);
    int getMovingObjectIndex(MovingObject* obj);

    void addStaticHitbox(Hitbox* hitbox);
    void addImgui(IGui* imgui);
    void addMovingObject(MovingObject* object);
    void addObject(std::shared_ptr<Object3D> object);
    void addDirLight(DirectionalLight* dirLight);
    void addPointLight(PointLight* pointLight);
    void addSpotLight(SpotLight* spotLight);
    void addDynamicHitbox(Hitbox* hitbox);
    void removeDynamicHitbox(Hitbox* hitbox);
    void addAnt(Ant* ant);

    void logStaticHitboxes();
    void logDynamicHitboxes();


    void removeAnt(Ant* ant);

    void removeStaticHitbox(Hitbox* hitbox);
    void removeImgui(IGui* igui);
    void removeMovingObject(MovingObject* object);
    void removeObject(std::shared_ptr<Object3D> object);
    void removeDirLight(DirectionalLight* dirLight);
    void removePointLight(PointLight* pointLight);
    void removeSpotLight(SpotLight* spotLight);


    void renderHitboxes(const glm::mat4& projectionView);
    void renderImgui();
    void renderLights(Shader shader);
    void renderDirLights(Shader shader);
    void renderPointLights(Shader shader);
    void renderSpotLights(Shader shader);

    void moveObjects();
    void drawObjects(const Camera& camera);

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
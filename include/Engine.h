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
    int getObject3DIndex(const std::shared_ptr<Object3D>& obj);
    std::shared_ptr<Object3D> getObject3DById(int id);
    int getMovingObjectIndex(const std::shared_ptr<MovingObject>& obj);

    void addStaticHitbox(const std::shared_ptr<Hitbox>& hitbox);
    void addImgui(const std::shared_ptr<IGui>& imgui);
    void addMovingObject(const std::shared_ptr<MovingObject>& object);
    void addObject(const std::shared_ptr<Object3D>& object);
    void addDirLight(const std::shared_ptr<DirectionalLight>& dirLight);
    void addPointLight(const std::shared_ptr<PointLight>& pointLight);
    void addSpotLight(const std::shared_ptr<SpotLight>& spotLight);
    void addDynamicHitbox(const std::shared_ptr<Hitbox>& hitbox);
    void removeDynamicHitbox(const std::shared_ptr<Hitbox>& hitbox);
    void addAnt(const std::shared_ptr<Ant>& ant);

    void logStaticHitboxes();
    void logDynamicHitboxes();


    void removeAnt(const std::shared_ptr<Ant>& ant);

    void removeStaticHitbox(const std::shared_ptr<Hitbox>& hitbox);
    void removeImgui(const std::shared_ptr<IGui>& igui);
    void removeMovingObject(const std::shared_ptr<MovingObject>& object);
    void removeObject(const std::shared_ptr<Object3D>& object);
    void removeDirLight(const std::shared_ptr<DirectionalLight>& dirLight);
    void removePointLight(const std::shared_ptr<PointLight>& pointLight);
    void removeSpotLight(const std::shared_ptr<SpotLight>& spotLight);


    void renderHitboxes(const glm::mat4& projectionView);
    void renderImgui();
    void renderLights(const Shader& shader);
    void renderDirLights(const Shader& shader);
    void renderPointLights(const Shader& shader);
    void renderSpotLights(const Shader& shader);

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
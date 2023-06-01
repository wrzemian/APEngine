#ifndef ENGINE_H
#define ENGINE_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"
#include "spdlog/spdlog.h"

#include <vector>
#include <iostream>
#include <memory>

#include "User/InputSystem.h"
#include "Objects/Ant.h"
#include "Parser.h"

class Engine {
//private:
public:
    static std::unique_ptr<Engine> instance;

    Engine();

    Engine(const Engine&) = delete;

    Engine& operator=(const Engine&) = delete;

    ~Engine();

    static GLFWwindow* window;

    Parser parser;

    float frameEnd;
    float frameStart;
    float deltaTime;
    int Weights;
    unsigned int displayCounter;
    unsigned int totalCounter;

    static int SCR_WIDTH;
    static int SCR_HEIGHT;

    bool frustum;

    std::vector<Hitbox*> staticHitboxes;
    std::vector<Hitbox*> dynamicHitboxes;

    std::vector<IGui*> allImgui;
    std::vector<Object3D*> allObjects;
    std::vector<MovingObject*> allMovingObjects;
    std::vector<DirectionalLight*> allDirLights;
    std::vector<PointLight*> allPointLights;
    std::vector<SpotLight*> allSpotLights;
    std::vector<Ant*> allAnts;
    std::vector<std::pair<Hitbox*, Hitbox*>> previousCollisions;

    bool renderDynamic;
    bool renderStatic;

    int renderedStatic;
    int renderedDynamic;

    int Init();

    int getObject3DIndex(Object3D* obj);

    void logStaticHitboxes();

    void logDynamicHitboxes();

    Object3D* getObject3DById(int id);

    int getMovingObjectIndex(MovingObject* obj);

    void addStaticHitbox(Hitbox* hitbox);

    void removeStaticHitbox(Hitbox* hitbox);

    void addDynamicHitbox(Hitbox* hitbox);

    void removeDynamicHitbox(Hitbox* hitbox);

    void addImgui(IGui* imgui);

    void removeImgui(IGui* igui);

    void addMovingObject(MovingObject* object);

    void removeMovingObject(MovingObject* object);

    void addObject(Object3D* object);

    void removeObject(Object3D* object);

    void addDirLight(DirectionalLight* dirLight);

    void removeDirLight(DirectionalLight* dirLight);

    void addPointLight(PointLight* pointLight);

    void removePointLight(PointLight* pointLight);

    void addSpotLight(SpotLight* spotLight);

    void removeSpotLight(SpotLight* spotLight);

    void addAnt(Ant* ant);

    void removeAnt(Ant* ant);

    void renderDirLights(Shader shader);

    void renderPointLights(Shader shader);

    void renderSpotLights(Shader shader);

    void logTextures();

    int getImguiIndex();

    void moveObjects();

    void drawObjects(Shader& shader, const Camera& camera);

    void renderLights(Shader shader, Camera& camera);

    void drawObjects();

    void ImGui();

    void LoopStart();

    void renderImgui();

    void renderHitboxes(const glm::mat4& projectionView);

    void resolveCollisions();

    void LoopEnd();

    void terminate();

    void processInput(GLFWwindow* window);

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    int initGLandImGui();

public:
    static Engine& getInstance();

    static void destroyInstance();

    static GLFWwindow* getWindow();

    friend void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

#endif // ENGINE_H

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
private:
    Engine() = delete;
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    ~Engine() = delete;

public:
    static GLFWwindow* window;
    static Parser parser;
    static float frameEnd;
    static float frameStart;
    static float deltaTime;
    static int Weights;
    static unsigned int displayCounter;
    static unsigned int totalCounter;

    static int SCR_WIDTH;
    static int SCR_HEIGHT;

    static bool frustum;
    static std::vector<Hitbox*> staticHitboxes;
    static std::vector<Hitbox*> dynamicHitboxes;
    static std::vector<IGui*> allImgui;
    static std::vector<Object3D*> allObjects;
    static std::vector<MovingObject*> allMovingObjects;
    static std::vector<DirectionalLight*> allDirLights;
    static std::vector<PointLight*> allPointLights;
    static std::vector<SpotLight*> allSpotLights;
    static std::vector<Ant*> allAnts;
    static std::vector<std::pair<Hitbox*, Hitbox*>> previousCollisions;
    static bool renderDynamic;
    static bool renderStatic;
    static int renderedStatic;
    static int renderedDynamic;
    static int Init();
    static int getObject3DIndex(Object3D* obj);
    static void logStaticHitboxes();
    static void logDynamicHitboxes();
    static Object3D* getObject3DById(int id);
    static int getMovingObjectIndex(MovingObject* obj);
    static void addStaticHitbox(Hitbox* hitbox);
    static void removeStaticHitbox(Hitbox* hitbox);
    static void addDynamicHitbox(Hitbox* hitbox);
    static void removeDynamicHitbox(Hitbox* hitbox);
    static void addImgui(IGui* imgui);
    static void removeImgui(IGui* igui);
    static void addMovingObject(MovingObject* object);
    static void removeMovingObject(MovingObject* object);
    static void addObject(Object3D* object);
    static void removeObject(Object3D* object);
    static void addDirLight(DirectionalLight* dirLight);
    static void removeDirLight(DirectionalLight* dirLight);
    static void addPointLight(PointLight* pointLight);
    static void removePointLight(PointLight* pointLight);
    static void addSpotLight(SpotLight* spotLight);
    static void removeSpotLight(SpotLight* spotLight);
    static void addAnt(Ant* ant);
    static void removeAnt(Ant* ant);
    static void renderDirLights(Shader shader);
    static void renderPointLights(Shader shader);
    static void renderSpotLights(Shader shader);
    static void logTextures();
    static int getImguiIndex();
    static void moveObjects();
    static void drawObjects(Shader& shader, const Camera& camera);
    static void renderLights(Shader shader, Camera& camera);
    static void drawObjects();
    static void ImGui();
    static void LoopStart();
    static void renderImgui();
    static void renderHitboxes(const glm::mat4& projectionView);
    static void resolveCollisions();
    static void LoopEnd();
    static void terminate();
    static void processInput(GLFWwindow* window);
    static int initGLandImGui();

    friend void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};



#endif // ENGINE_H

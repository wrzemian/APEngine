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

#include "../include/Engine.h"

#include "../include/InputSystem.h"
#include "../include/Hitbox.h"
#include "../include/Object3D.h"
#include "../include/MovingObject.h"
#include "../include/FrustumOpitmizer.h"


namespace Engine {
    GLFWwindow* window;

    Parser parser("../../res/jsons");

    float frameEnd = 0;
    float frameStart = 0;
    float deltaTime = 0;

    unsigned int displayCounter = 0;
    unsigned int totalCounter = 0;

    int SCR_WIDTH = 1000;
    int SCR_HEIGHT = 800;

    std::vector<Hitbox*> allHitboxes;
    std::vector<IGui*> allImgui;
    std::vector<Object3D*> allObjects;
    std::vector<MovingObject*> allMovingObjects;
    std::vector<DirectionalLight*> allDirLights;
    std::vector<PointLight*> allPointLights;
    std::vector<SpotLight*> allSpotLights;

    int Init() {
        if (initGLandImGui() == -1) {
            return -1;
        }

        return 0;
    }

    int getObject3DIndex(Object3D* obj) {
        auto it = find(allObjects.begin(), allObjects.end(), obj);
        if (it != allObjects.end())
        {
            return it - allObjects.begin();

        }
        else {
            return -1;
        }
    }

    int getMovingObjectIndex(MovingObject* obj) {
        auto it = find(allMovingObjects.begin(), allMovingObjects.end(), obj);
        if (it != allMovingObjects.end())
        {
            return it - allMovingObjects.begin();

        }
        else {
            return -1;
        }
    }

    void addHitbox(Hitbox* hitbox) {
        spdlog::warn("Hitbox added");
        allHitboxes.push_back(hitbox);
    }

    void addImgui(IGui* imgui) {
        spdlog::warn("imgui object added, {}", imgui->windowName);
        allImgui.push_back(imgui);
    }

    void removeHitbox(Hitbox* hitbox) {
        spdlog::error("removing hitbox");
        std::erase(allHitboxes, hitbox);
    }

    void removeImgui(IGui* igui) {
        spdlog::error("removing igui, windowName = {}", igui->windowName);
        std::erase(allImgui, igui);
    }

    void addMovingObject(MovingObject* object) {
        spdlog::warn("moving object added, {}", object->windowName);
        allMovingObjects.push_back(object);
    }

    void removeMovingObject(MovingObject* object) {
        spdlog::warn("removing moving object");
        std::erase(allMovingObjects, object);
    }

    void addObject(Object3D* object) {
        spdlog::warn("object added, {}", object->windowName);
        allObjects.push_back(object);
    }

    void removeObject(Object3D* object) {
        spdlog::warn("moving object");
        std::erase(allObjects, object);
    }

    void addDirLight(DirectionalLight* dirLight) {
        spdlog::warn("adding dirLight");
        allDirLights.push_back(dirLight);
    }

    void removeDirLight(DirectionalLight* dirLight) {
        spdlog::warn("removing dirLight");
        std::erase(allDirLights, dirLight);
    }

    void addPointLight(PointLight* pointLight) {
        spdlog::warn("adding pointLight");
        allPointLights.push_back(pointLight);
    }

    void removePointLight(PointLight* pointLight) {
        spdlog::warn("removing pointLight");
        std::erase(allPointLights, pointLight);
    }

    void addSpotLight(SpotLight* spotLight) {
        spdlog::warn("adding spotLight");
        allSpotLights.push_back(spotLight);
    }

    void removeSpotLight(SpotLight* spotLight) {
        spdlog::warn("removing spotLight");
        std::erase(allSpotLights, spotLight);
    }

    void renderDirLights(Shader shader){
        for(DirectionalLight* dirLight: allDirLights) {
            dirLight->SendToShader(shader, "dirLight");
        }
    }

    void renderPointLights(Shader shader){
        for(PointLight* pointLight: allPointLights) {
            pointLight->SendToShader(shader, "pointLight");
        }
    }
    void renderSpotLights(Shader shader){
        for(SpotLight* spotLight: allSpotLights) {
            spotLight->SendToShader(shader, "spotLight");
        }
    }


    void renderLights(Shader shader) {
        renderDirLights(shader);
        renderPointLights(shader);
        renderSpotLights(shader);
    }

    int getImguiIndex() {
        return allImgui.size();
    }

    void moveObjects(){
        for(MovingObject* object: allMovingObjects) {
            object->Move();
        }
    }

    void drawObjects(Camera camera) {
        displayCounter = 0;
        totalCounter = 0;
        const Frustum camFrustum = Fru::createFrustumFromCamera(camera, (float)SCR_WIDTH / (float)SCR_HEIGHT, glm::radians(camera.Zoom), 0.1f, 100.0f);
        for(Object3D* object: allObjects) {
//                object->Draw();
            object->_transform.computeModelMatrix();
            Entity test(object->_model);
//            test.transform.setLocalPosition(object->_transform._position );
//            test.transform.setLocalScale(object->_transform._scale);
//            test.transform.setLocalRotation(object->_transform._rotation);

            if (test.boundingVolume->isOnFrustum(camFrustum, object->_transform))
            {
                object->Draw();
                displayCounter++;
            }
            totalCounter++;
        }
    }

    void ImGui() {
        ImGui::Begin("Engine");
        ImGui::SetWindowSize(ImVec2(200, 100));
        //spdlog::info("deltaTime: ", Engine::deltaTime);

        ImGui::Text("deltaTime: %f", Engine::deltaTime);
        ImGui::Text("FPS: %f", 1.0f / Engine::deltaTime);
        ImGui::Text("displayed: %d", displayCounter);
        ImGui::Text("total: %d", totalCounter);
        ImGui::End();
    }


    void LoopStart() {
        frameEnd = glfwGetTime();

        deltaTime = frameEnd - frameStart;

        frameStart = glfwGetTime();

        // render loop
        // -----------

        // input
        // -----
        processInput(window);


        //spdlog::info("frameEnd: {:03.20f}", frameEnd);
        //spdlog::info("frameStart: {:03.20f}", frameStart);
        //spdlog::critical("deltaTime: {:03.20f}", deltaTime);

        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_LIGHT0);
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    }

    void renderImgui() {
        for(IGui* gui: allImgui) {
            gui->ImGui();
        }
    }

    void renderHitboxes(const glm::mat4& projectionView) {
        for(Hitbox* hitbox: allHitboxes) {
            hitbox->Draw(projectionView);
        }
    }

    void resolveCollisions() {

        for(size_t i=0; i<allHitboxes.size(); i++) {
            for (size_t j = i+1; j < allHitboxes.size(); j++) {
                allHitboxes.at(i)->TestForIntersection(*allHitboxes.at(j));
            }
        }
    }

    void LoopEnd() {
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void terminate() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        // glfw: terminate, clearing all previously allocated GLFW resources.
        // ------------------------------------------------------------------
        glfwTerminate();
    }




    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void processInput(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }

    int initGLandImGui() {
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
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }
        glEnable(GL_DEPTH_TEST);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 430");
        ImGui::StyleColorsClassic();

        return 0;
    }

    GLFWwindow* getWindow()
    {
        return window;
    }

}
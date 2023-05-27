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

#include "../include/User/InputSystem.h"
#include "../include/FrustumOpitmizer.h"
#include "../include/Objects/Ant.h"


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

    bool frustum = true;

    std::vector<Hitbox*> staticHitboxes;
    std::vector<Hitbox*> dynamicHitboxes;

    std::vector<IGui*> allImgui;
    std::vector<Object3D*> allObjects;
    std::vector<MovingObject*> allMovingObjects;
    std::vector<DirectionalLight*> allDirLights;
    std::vector<PointLight*> allPointLights;
    std::vector<SpotLight*> allSpotLights;
    std::vector<Ant*> allAnts;

    bool renderDynamic = false;
    bool renderStatic = false;

    int renderedStatic = 0;
    int renderedDynamic = 0;

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

    void logStaticHitboxes() {
        spdlog::warn("Static hitboxes:");
        for(auto h: staticHitboxes) {
            spdlog::info("{}", h->_object->tag);
        }
    }

    void logDynamicHitboxes() {
        spdlog::warn("Dynamic hitboxes:");
        for(auto h: dynamicHitboxes) {
            spdlog::info("{}", h->_object->tag);
        }
    }


    Object3D* getObject3DById(int id) {
            return allObjects.at(id);
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

    void addStaticHitbox(Hitbox* hitbox) {
        //spdlog::info("Static hitbox added");
        staticHitboxes.push_back(hitbox);
    }

    void removeStaticHitbox(Hitbox* hitbox) {
        //spdlog::info("removing static hitbox");
        std::erase(staticHitboxes, hitbox);
    }

    void addDynamicHitbox(Hitbox* hitbox) {
        spdlog::info("Dynamic hitbox added, {}", hitbox->getWindowName());
        dynamicHitboxes.push_back(hitbox);
    }

    void removeDynamicHitbox(Hitbox* hitbox) {
        //spdlog::info("removing dynamic hitbox");
        std::erase(dynamicHitboxes, hitbox);
    }

    void addImgui(IGui* imgui) {
        //spdlog::warn("imgui object added, {}", imgui->_windowName);
        allImgui.push_back(imgui);
    }

    void removeImgui(IGui* igui) {
        //spdlog::error("removing igui, _windowName = {}", igui->_windowName);
        std::erase(allImgui, igui);
    }

    void addMovingObject(MovingObject* object) {
        //spdlog::warn("moving object added, {}", object->_windowName);
        allMovingObjects.push_back(object);
    }

    void removeMovingObject(MovingObject* object) {
        //spdlog::warn("removing moving object");
        std::erase(allMovingObjects, object);
    }

    void addObject(Object3D* object) {
        //spdlog::warn("object added, {}", object->_windowName);
        allObjects.push_back(object);
    }

    void removeObject(Object3D* object) {
        //spdlog::warn("removing object");
        std::erase(allObjects, object);
    }

    void addDirLight(DirectionalLight* dirLight) {
        //spdlog::warn("adding dirLight");
        allDirLights.push_back(dirLight);
    }

    void removeDirLight(DirectionalLight* dirLight) {
        //spdlog::warn("removing dirLight");
        std::erase(allDirLights, dirLight);
    }

    void addPointLight(PointLight* pointLight) {
        //spdlog::warn("adding pointLight");
        allPointLights.push_back(pointLight);
    }

    void removePointLight(PointLight* pointLight) {
        //spdlog::warn("removing pointLight");
        std::erase(allPointLights, pointLight);
    }

    void addSpotLight(SpotLight* spotLight) {
        //spdlog::warn("adding spotLight");
        allSpotLights.push_back(spotLight);
    }

    void removeSpotLight(SpotLight* spotLight) {
       // spdlog::warn("removing spotLight");
        std::erase(allSpotLights, spotLight);
    }

    void addAnt(Ant* ant) {
        //spdlog::warn("adding spotLight");
        allAnts.push_back(ant);
    }

    void removeAnt(Ant* ant) {
        // spdlog::warn("removing spotLight");
        std::erase(allAnts, ant);
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
        if(deltaTime > 0.01) {
            deltaTime = 0.01f;
        }
        for(MovingObject* object: allMovingObjects) {
            object->Move(deltaTime);
        }
    }

    void drawObjects(const Camera& camera) {
        displayCounter = 0;
        totalCounter = 0;

        const Frustum camFrustum = Fru::createFrustumFromCamera(camera, (float)SCR_WIDTH / (float)SCR_HEIGHT, glm::radians(camera.Zoom), 0.1f, 100.0f);
        for(Object3D* object: allObjects) {
//            object->_transform.computeModelMatrix();
            Entity test(*object->_model);
            test.transform.setLocalPosition(object->_transform._position);
            test.transform.setLocalScale(object->_transform._scale);
            test.transform.setLocalRotation(object->_transform._rotation);
            test.transform.computeModelMatrix();
//            if (test.boundingVolume->isOnFrustum(camFrustum, object->_transform))
            if(frustum) {
                if (test.boundingVolume->isOnFrustum(camFrustum, test.transform))
                {
                    object->Draw();
                    displayCounter++;
                }
            }
            else {
                object->Draw();
                displayCounter++;
            }
            totalCounter++;
        }

    }

    void ImGui() {
        ImGui::Begin("Engine");
        ImGui::SetWindowSize(ImVec2(250, 300));
        //spdlog::info("deltaTime: ", Engine::deltaTime);

        ImGui::Text("deltaTime: %f", Engine::deltaTime);
        ImGui::Text("FPS: %f", 1.0f / Engine::deltaTime);
        ImGui::Text("displayed: %d", displayCounter);
        ImGui::Text("total: %d", totalCounter);
        ImGui::Checkbox("FRUSTUM", &frustum);

        ImGui::SliderInt("static", &renderedStatic, 0, (int)staticHitboxes.size() - 1);
        for (const auto& hitbox : staticHitboxes) {
            hitbox->draw = false;
        }
        staticHitboxes.at(renderedStatic)->draw = true;

        ImGui::SliderInt("dynamic", &renderedDynamic, 0, (int)dynamicHitboxes.size() - 1);
        for (const auto& hitbox : dynamicHitboxes) {
            hitbox->draw = false;
        }
        dynamicHitboxes.at(renderedDynamic)->draw = true;

        ImGui::Checkbox("Show static hitboxes", &renderStatic);
        if (renderStatic) {
            for (const auto& hitbox : staticHitboxes) {
                hitbox->draw = true;
            }
        }

        ImGui::Checkbox("Show dynamic hitboxes", &renderDynamic);
        if (renderDynamic) {
            for (const auto& hitbox : dynamicHitboxes) {
                hitbox->draw = true;
            }
        }
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
            if(gui->isShownImgui()) {
                gui->ImGui();
            }
        }
    }

    void renderHitboxes(const glm::mat4& projectionView) {
        for(auto hitbox: staticHitboxes) {
            hitbox->Draw(projectionView);
        }
        for(auto hitbox: dynamicHitboxes) {
            hitbox->Draw(projectionView);
        }
    }

    void resolveCollisions() {
        for(auto dynamicHitbox: dynamicHitboxes) {
            for(auto staticHitbox: staticHitboxes) {
                dynamicHitbox->TestForIntersection(staticHitbox);
            }
        }

        for (int i = 0; i < dynamicHitboxes.size(); i++) {
            for (int j = i + 1; j < dynamicHitboxes.size(); j++) {
                if(dynamicHitboxes.at(i)->_object != dynamicHitboxes.at(j)->_object) {
                    dynamicHitboxes.at(j)->TestForIntersection(dynamicHitboxes.at(i));
                }
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
#ifdef __APPLE__
        ImGui_ImplOpenGL3_Init("#version 330");
#else
        ImGui_ImplOpenGL3_Init("#version 430");
#endif
        ImGui::StyleColorsClassic();

        return 0;
    }

    GLFWwindow* getWindow()
    {
        return window;
    }

}
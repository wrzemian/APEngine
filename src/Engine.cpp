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



    int Engine::Init() {
        if (initGLandImGui() == -1) {
            return -1;
        }

        return 0;
    }

    int Engine::getObject3DIndex(Object3D* obj) {
        auto it = find(allObjects.begin(), allObjects.end(), obj);
        if (it != allObjects.end())
        {
            return it - allObjects.begin();

        }
        else {
            return -1;
        }
    }

    void Engine::logStaticHitboxes() {
        spdlog::warn("Static hitboxes:");
        for(auto h: staticHitboxes) {
            spdlog::info("{}", h->_object->tag);
        }
    }

    void Engine::logDynamicHitboxes() {
        spdlog::warn("Dynamic hitboxes:");
        for(auto h: dynamicHitboxes) {
            spdlog::info("{}", h->_object->tag);
        }
    }


    Object3D* Engine::getObject3DById(int id) {
        std::cout<< allObjects.size() <<std::endl;
        return allObjects.at(id);
    }

    int Engine::getMovingObjectIndex(MovingObject* obj) {
        auto it = find(allMovingObjects.begin(), allMovingObjects.end(), obj);
        if (it != allMovingObjects.end())
        {
            return it - allMovingObjects.begin();

        }
        else {
            return -1;
        }
    }

    void Engine::addStaticHitbox(Hitbox* hitbox) {
        //spdlog::info("Static hitbox added");
        staticHitboxes.push_back(hitbox);
    }

    void Engine::removeStaticHitbox(Hitbox* hitbox) {
        //spdlog::info("removing static hitbox");
        std::erase(staticHitboxes, hitbox);
    }

    void Engine::addDynamicHitbox(Hitbox* hitbox) {
        //spdlog::info("Dynamic hitbox added, {}", hitbox->getWindowName());
        dynamicHitboxes.push_back(hitbox);
    }

    void Engine::removeDynamicHitbox(Hitbox* hitbox) {
        //spdlog::info("removing dynamic hitbox");
        std::erase(dynamicHitboxes, hitbox);
    }

    void Engine::addImgui(IGui* imgui) {
        //spdlog::warn("imgui object added, {}", imgui->_windowName);
        allImgui.push_back(imgui);
    }

    void Engine::removeImgui(IGui* igui) {
        //spdlog::error("removing igui, _windowName = {}", igui->_windowName);
        std::erase(allImgui, igui);
    }

    void Engine::addMovingObject(MovingObject* object) {
        //spdlog::warn("moving object added, {}", object->_windowName);
        allMovingObjects.push_back(object);
    }

    void Engine::removeMovingObject(MovingObject* object) {
        //spdlog::warn("removing moving object");
        std::erase(allMovingObjects, object);
    }

    void Engine::addObject(Object3D* object) {
        //spdlog::warn("object added, {}", object->_windowName);
        allObjects.push_back(object);
    }

    void Engine::removeObject(Object3D* object) {
        //spdlog::warn("removing object");
        std::erase(allObjects, object);
    }

    void Engine::addDirLight(DirectionalLight* dirLight) {
        //spdlog::warn("adding dirLight");
        allDirLights.push_back(dirLight);
    }

    void Engine::removeDirLight(DirectionalLight* dirLight) {
        //spdlog::warn("removing dirLight");
        std::erase(allDirLights, dirLight);
    }

    void Engine::addPointLight(PointLight* pointLight) {
        //spdlog::warn("adding pointLight");
        allPointLights.push_back(pointLight);
    }

    void Engine::removePointLight(PointLight* pointLight) {
        //spdlog::warn("removing pointLight");
        std::erase(allPointLights, pointLight);
    }

    void Engine::addSpotLight(SpotLight* spotLight) {
        //spdlog::warn("adding spotLight");
        allSpotLights.push_back(spotLight);
    }

    void Engine::removeSpotLight(SpotLight* spotLight) {
       // spdlog::warn("removing spotLight");
        std::erase(allSpotLights, spotLight);
    }

    void Engine::addAnt(Ant* ant) {
        //spdlog::warn("adding spotLight");
        allAnts.push_back(ant);
    }

    void Engine::removeAnt(Ant* ant) {
        // spdlog::warn("removing spotLight");
        std::erase(allAnts, ant);
    }

    void Engine::renderDirLights(Shader shader){
        for(DirectionalLight* dirLight: allDirLights) {
            dirLight->SendToShader(shader, "dirLight");
        }
    }

    void Engine::renderPointLights(Shader shader){
        for(PointLight* pointLight: allPointLights) {
            pointLight->SendToShader(shader, "pointLight");
        }
    }
    void Engine::renderSpotLights(Shader shader){
        for(SpotLight* spotLight: allSpotLights) {
            spotLight->SendToShader(shader, "spotLight");
        }
    }


    void Engine::logTextures() {

    }

    int Engine::getImguiIndex() {
        return allImgui.size();
    }

    void Engine::moveObjects(){
        if(deltaTime > 0.01) {
            deltaTime = 0.01f;
        }
        for(MovingObject* object: allMovingObjects) {
            object->Move(deltaTime);
        }
    }

    void Engine::drawObjects(Shader &shader, const Camera& camera) {
        displayCounter = 0;
        totalCounter = 0;

        const Frustum camFrustum = Fru::createFrustumFromCamera(camera, (float)SCR_WIDTH / (float)SCR_HEIGHT, glm::radians(camera.Zoom), 0.1f, 100.0f);
        for(Object3D* object: allObjects) {
            spdlog::info("rendering {}", object->tag);
            if(!object->rendered) {
                continue;
            }
            if(object->_model == nullptr) {
                spdlog::error("object {}/{} has null model", object->tag, object->getWindowName());
                object->setShader(&shader);
            }
            if(object->_shader != &shader) {
                object->setShader(&shader);
            }
//                object->Draw();
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

    void Engine::renderLights(Shader shader, Camera& camera) {
        shader.setMat4("projectionView", camera.getViewProjection());
        glm::mat4 model = glm::mat4 (1.0f);
        shader.setMat4("model", model);
        drawObjects(shader, camera);

        shader.setInt("material.diffuse", 0);
        shader.setInt("material.specular", 1);
        shader.setFloat("material.shininess", 32.0f);
        //from camera
        shader.setVec3("viewPos", camera.Position);
        renderDirLights(shader);
        renderPointLights(shader);
        renderSpotLights(shader);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)Engine::SCR_WIDTH / (float)Engine::SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projectionView", projection * view);

    }

    void Engine::drawObjects() {
        for(Object3D* object: allObjects) {
            object->Draw();
        }

    }

    void Engine::ImGui() {
        ImGui::Begin("Engine");
        ImGui::SetWindowSize(ImVec2(250, 300));
        //spdlog::info("deltaTime: ", Engine::deltaTime);

        ImGui::Text("deltaTime: %f", Engine::deltaTime);
        ImGui::Text("FPS: %f", 1.0f / Engine::deltaTime);
        ImGui::Text("displayed: %d", displayCounter);
        ImGui::Text("total: %d", totalCounter);
        ImGui::Checkbox("FRUSTUM", &frustum);

        ImGui::SliderInt("Static", &renderedStatic, -1, (int)staticHitboxes.size() - 1);
        ImGui::SliderInt("Dynamic", &renderedDynamic, -1, (int)dynamicHitboxes.size() - 1);

        ImGui::Checkbox("Show static hitboxes", &renderStatic);
        ImGui::Checkbox("Show dynamic hitboxes", &renderDynamic);

        for (const auto& hitbox : staticHitboxes) {
            hitbox->draw = renderStatic;
        }
        for (const auto& hitbox : dynamicHitboxes) {
            hitbox->draw = renderDynamic;
        }

        if(renderedStatic >= 0)
            staticHitboxes.at(renderedStatic)->draw = true;

        if(renderedDynamic >= 0)
            dynamicHitboxes.at(renderedDynamic)->draw = true;

        ImGui::End();
    }


    void Engine::LoopStart() {
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

    void Engine::renderImgui() {
        for(IGui* gui: allImgui) {
            if(gui->isShownImgui()) {
                gui->ImGui();
            }
        }
    }

    void Engine::renderHitboxes(const glm::mat4& projectionView) {
        for(auto hitbox: staticHitboxes) {
            hitbox->Draw(projectionView);
        }
        for(auto hitbox: dynamicHitboxes) {
            hitbox->Draw(projectionView);
        }
    }

    void Engine::resolveCollisions() {
        std::vector<std::pair<Hitbox*, Hitbox*>> currentCollisions;

        // Check for collisions between dynamic and static hitboxes
        for(auto dynamicHitbox: dynamicHitboxes) {
            for(auto staticHitbox: staticHitboxes) {
                if(dynamicHitbox->TestForIntersection(staticHitbox)) {
                    currentCollisions.push_back({dynamicHitbox, staticHitbox});
                }
            }
        }

        // Check for collisions between pairs of dynamic hitboxes
        for (int i = 0; i < dynamicHitboxes.size(); i++) {
            for (int j = i + 1; j < dynamicHitboxes.size(); j++) {
                if(dynamicHitboxes.at(i)->_object != dynamicHitboxes.at(j)->_object) {
                    if(dynamicHitboxes.at(j)->TestForIntersection(dynamicHitboxes.at(i))) {
                        currentCollisions.push_back({dynamicHitboxes.at(i), dynamicHitboxes.at(j)});
                    }
                }
            }
        }

        // Now, check for collisions that have just ended
        for(auto& collision : previousCollisions) {
            // Do a linear search to find collision in currentCollisions
            auto it = std::find(currentCollisions.begin(), currentCollisions.end(), collision);

            if(it == currentCollisions.end()) {
                // The objects are no longer colliding
                collision.first->_object->onCollisionExit(collision.second->_object);
                collision.second->_object->onCollisionExit(collision.first->_object);
            }
        }

        // Finally, update the set of previous collisions for the next frame
        previousCollisions = std::move(currentCollisions);
    }

    void Engine::LoopEnd() {
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void Engine::terminate() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        // glfw: terminate, clearing all previously allocated GLFW resources.
        // ------------------------------------------------------------------
        glfwTerminate();
    }




    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void Engine::processInput(GLFWwindow* window)
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

    int Engine::initGLandImGui() {
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

    GLFWwindow* Engine::getWindow()
    {
        return window;
    }




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

#include "../include/MovingObject.h"
#include "../include/Object3D.h"

namespace Engine {
    GLFWwindow* window;

    float frameEnd = 0;
    float frameStart = 0;
    float deltaTime = 0;

    const int SCR_WIDTH = 1000;
    const int SCR_HEIGHT = 800;

    std::vector<Hitbox*> allHitboxes;
    std::vector<IGui*> allImgui;
    std::vector<Object3D*> allObjects;
    std::vector<MovingObject*> allMovingObjects;

    int Init() {
        if (initGLandImGui() == -1) {
            return -1;
        }
        return 0;
    }

    void addHitbox(Hitbox* hitbox) {
        spdlog::info("Hitbox added");
        allHitboxes.push_back(hitbox);
    }

    void addImgui(IGui* imgui) {
        spdlog::info("imgui object added, {}", imgui->windowName);
        allImgui.push_back(imgui);
    }

    void addObject(Object3D* object) {
        spdlog::info("Object added");
        allObjects.push_back(object);
    }

    void addMovingObject(MovingObject* object) {
        spdlog::info("Moving Object added");
        allMovingObjects.push_back(object);
    }

    int getImguiIndex() {
        return allImgui.size();
    }

    void LoopStart() {
        frameEnd = glfwGetTime();

        deltaTime = frameEnd - frameStart;

        frameStart = glfwGetTime();

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_LIGHT0);
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    }

    void resolveCollisions() {
        for(size_t i=0; i<allHitboxes.size(); i++) {
            for (size_t j = i+1; j < allHitboxes.size(); j++) {
                allHitboxes.at(i)->TestForIntersection(*allHitboxes.at(j));
            }
        }
    }

    void LoopEnd() {
        std::cout<<"imgui przed \n";
       // TODO: uncomment
       //  ImGui();
        std::cout<<"imgui po \n";
        moveObjects();
        resolveCollisions();
        drawObjects();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
       // TODO: uncomment
       //  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void moveObjects() {
        for(MovingObject* object: allMovingObjects) {
            object->Move(deltaTime);
        }
    }

    void drawObjects() {
        for(Object3D* object: allObjects) {
            std::cout<<"DRAW\n";
            object->Draw();
        }
    }

    void ImGui() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            renderImgui();
            framerate();
        }
        ImGui::Render();
    }

    void framerate() {
        ImGui::Begin("Engine");
        ImGui::SetWindowSize(ImVec2(200, 100));
        //spdlog::info("deltaTime: ", Engine::deltaTime);

        ImGui::Text("deltaTime: %f", Engine::deltaTime);
        ImGui::Text("FPS: %f", 1.0f / Engine::deltaTime);

        ImGui::End();
    }

    void renderImgui() {
        for(IGui* gui: allImgui) {
            gui->ImGui();
        }
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
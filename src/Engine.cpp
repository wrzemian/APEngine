#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"

#include <vector>
#include <iostream>

#include "../include/Engine.h"

namespace Engine {

    GLFWwindow* window;

    const int SCR_WIDTH = 1000;
    const int SCR_HEIGHT = 800;

    int Init()
    {

        if (initGLandImGui() == -1) {
            return -1;
        }
        return 0;
    }



    void LoopStart() {
        // render loop
        // -----------

        // input
        // -----
        processInput(window);


        // draw pyramids
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_LIGHT0);
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    }

    void LoopEnd() {
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void terminate() {
        //renderer.deleteBuffers();


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

    void renderImGuiUI(int* depth, glm::vec2* rotate, glm::vec3* color, const int MAX_DEPTH, glm::fvec3* direction) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Recursion");
            ImGui::SetWindowPos(ImVec2(0, 0));
            ImGui::SetWindowSize(ImVec2(250, 50));

            ImGui::SliderInt("Depth", depth, 0, MAX_DEPTH);
            ImGui::End();


            ImGui::Begin("Rotation");
            ImGui::SetWindowPos(ImVec2(0, 50));
            ImGui::SetWindowSize(ImVec2(250, 70));

            ImGui::SliderFloat("X", &rotate->x, -360.0f, 360.0f);
            ImGui::SliderFloat("Y", &rotate->y, -360.0f, 360.0f);
            ImGui::End();


            ImGui::Begin("Color");
            ImGui::SetWindowPos(ImVec2(0, 120));
            ImGui::SetWindowSize(ImVec2(250, 100));

            ImGui::SliderFloat("R", &color->x, 0.0f, 1.0f);
            ImGui::SliderFloat("G", &color->y, 0.0f, 1.0f);
            ImGui::SliderFloat("B", &color->z, 0.0f, 1.0f);
            ImGui::End();

            ImGui::Begin("Direction");
            ImGui::SliderFloat3("DIR", &direction->x, -1.0f, 1.0f);
            ImGui::End();
        }
        ImGui::Render();
    }

//    void checkInit() {
//            if (!gladLoadGL()) {
//                std::cerr << "Failed to initialize OpenGL context" << std::endl;
//                exit(EXIT_FAILURE);
//            }
//
//    }

}
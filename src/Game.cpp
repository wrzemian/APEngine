#include <iostream>


#include "../include/Game.h"

#include "../include/Engine.h"

#include "../include/Object3D.h"
#include "../include/MovingObject.h"

#include "../include/lights/DirectionalLight.h"
#include "../include/Camera.h"
#include "../include/DebugShape.h"

#include "../include/lights/SpotLight.h"
#include "../include/lights/PointLight.h"

#include "../include/Parser.h"
#include "../include/Hitbox.h"
#include "../include/InputSystem.h"
#include "../include/HUD.h"
#include "../include/Walls.h"

#include "spdlog/spdlog.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "al.h"
#include "alc.h"

namespace Game {
    void processInput(GLFWwindow* window);

    void ImGui();

    HUD hud;
    Shader shader;

    MovingObject movingObject;
    Walls wagon;

    Camera camera(glm::vec3(0.f, 5.0f, 30.0f));
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    DirectionalLight dirLight;
    SpotLight spotLight;
    PointLight pointLight;

    InputSystem inputSystem;


    void Start() {
        std::cout << Engine::Init() <<"\n";

        ALCdevice *device;
        device = alcOpenDevice(NULL);

        inputSystem.InputInit();
        inputSystem.monitorKey(GLFW_KEY_A);
        inputSystem.monitorKey(GLFW_KEY_SPACE);

        hud.initAnimation();
        hud.initText("res/fonts/Arialn.ttf");
        movingObject.loadModel("../../res/models/first_character/first character.obj");

        wagon.loadModel("../../res/models/1level/1level.obj");
        wagon.calculateHitboxes();
        wagon.logHitboxes();
//        for(auto m: wagon._model.meshes) {
//            spdlog::info("loaded mesh with {} vertices", m.vertices.size());
//        }

        // build and compile our shader program
        // ------------------------------------
        Shader temp("../../res/shaders/shader.vert", "../../res/shaders/shader.frag");
        shader = temp;
        shader.use();

        movingObject.setShader(&shader);
        wagon.setShader(&shader);

        shader.setMat4("projectionView", camera.viewProjection);

        glm::mat4 model = glm::mat4 (1.0f);
        shader.setMat4("model", model);

        Parser parser("../../res/jsons");

        dirLight = parser.CreateFromJSONDir("dirLight");
        spotLight = parser.CreateFromJSONSpot("spotLight");
        pointLight = parser.CreateFromJSONPoint("pointLight");

        spdlog::info("ImGui");
        ImGui();
        spdlog::info("ResolveCollisions");
        Engine::resolveCollisions();
        spdlog::info("loop");

        while (!glfwWindowShouldClose(Engine::getWindow())) {
             Update();
        }
    }

    void Update() {
        Engine::LoopStart();
        ImGui();

        inputSystem.update();



        //movingObject.Move();

        Engine::moveObjects();
        Engine::drawObjects();


        //movingObject.Draw();
        //wagon.Draw();

        //box.Draw(shader);

        ////REQUIRED FOR LIGHT
        //should be property of object
        shader.setInt("material.diffuse", 0);
        shader.setInt("material.specular", 1);
        shader.setFloat("material.shininess", 32.0f);
        //from camera
        shader.setVec3("viewPos", glm::vec3(0.0f, 0.0f, 1.0f));

        dirLight.SendToShader(shader, "dirLight");
        spotLight.SendToShader(shader, "spotLight");
        pointLight.SendToShader(shader, "pointLight");


        processInput(Engine::getWindow());


        //camera

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        glm:: mat4 view = camera.getView(movingObject);

        shader.use();
        shader.setMat4("projectionView", projection * view);

        Engine::renderHitboxes(projection * view);

        camera.followObject(movingObject);

        Engine::resolveCollisions();
        Engine::LoopEnd();

    }

    void ImGui() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            Engine::renderImgui();
            Engine::ImGui();

        }
        ImGui::Render();
    }


    void processInput(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, Engine::deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, Engine::deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, Engine::deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, Engine::deltaTime);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera.Position.y += 0.05;
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
            camera.Position.y -= 0.05;
    }

};
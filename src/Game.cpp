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

#include "spdlog/spdlog.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "al.h"
#include "alc.h"

//void processInput(GLFWwindow* window);


namespace Game {
    void processInput(GLFWwindow* window);

    void ImGui();

    const int SCR_WIDTH = 1000;
    const int SCR_HEIGHT = 800;

    GLFWwindow* window;
    HUD hud;
    Shader shader;

//    Object3D floor;
//    Object3D box;
    MovingObject movingObject;

    Object3D walls;
    Object3D wagon;

    Camera camera(glm::vec3(0.f, 5.0f, 30.0f));
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    DirectionalLight dirLight;
    SpotLight spotLight;
    PointLight pointLight;

    InputSystem inputSystem;

    Hitbox hitbox1;
    Hitbox floor;
    //DebugShape debugShape;
float x = 0;
float y = 0;

    void Start() {
        std::cout << Engine::Init();


        ALCdevice *device;

        device = alcOpenDevice(NULL);

        window = Engine::getWindow();
        inputSystem.InputInit();
        inputSystem.monitorKey(GLFW_KEY_A);
        inputSystem.monitorKey(GLFW_KEY_SPACE);

//        hud.initImage("res/textures/tlo.png");
        hud.initAnimation();
        hud.initText("res/fonts/Arialn.ttf");
//        floor.loadModel("../../res/models/first box/skrzynia.obj");
//        floor._transform._scale = glm::vec3(50, 1, 50);
//        box.loadModel("../../res/models/second box/skrzynia2.obj");

        movingObject.loadModel("../../res/models/first_character/first character.obj");
        movingObject._transform._scale = glm::vec3(0.2f);
        walls.loadModel("../../res/models/walls/walls.obj");
        wagon.loadModel("../../res/models/1level/1level.obj");

        hitbox1.Create(&movingObject, glm::vec3(1,3,2));
        hitbox1._min = glm::vec3(-1, -1, -1);
        hitbox1._max = glm::vec3(1, 1, 1);

        Object3D f;
        floor.Create(&f);
        floor._min = glm::vec3(-20, -5, -20);
        floor._max = glm::vec3(20, 0, 20);
        floor._color = glm::vec3(1,1,0);

        // build and compile our shader program
        // ------------------------------------
        Shader temp("../../res/shaders/shader.vert", "../../res/shaders/shader.frag");
        shader = temp;
        shader.use();

        movingObject.setShader(&shader);
        walls.setShader(&shader);
        wagon.setShader(&shader);

        shader.setMat4("projectionView", camera.viewProjection);

        glm::mat4 model = glm::mat4 (1.0f);
        shader.setMat4("model", model);

        Parser parser("../../res/jsons");

        dirLight = parser.CreateFromJSONDir("dirLight");
        spotLight = parser.CreateFromJSONSpot("spotLight");
        pointLight = parser.CreateFromJSONPoint("pointLight");


        //parser.SaveJSON(dirLight.ParseToJSON(), "dirLight");
//        parser.SaveJSON(spotLight.ParseToJSON(), "spotLight");
//        parser.SaveJSON(pointLight.ParseToJSON(), "pointLight");


        while (!glfwWindowShouldClose(window)) {
             Update();
        }
    }

    void Update() {

        Engine::LoopStart();
        ImGui();
        x -= 0.3;


        inputSystem.update();

        if (inputSystem.GetKey(GLFW_KEY_A))
            std::cout << "escape1" << std::endl; // we have library for logging...
        if (inputSystem.GetKeyDown(GLFW_KEY_A))
            std::cout << "aaaa" << std::endl;
/*
        float xAxisGamepad1 = inputSystem.getJoystickAxis(0, GLFW_GAMEPAD_AXIS_LEFT_X);
        float yAxisGamepad1 = inputSystem.getJoystickAxis(0, GLFW_GAMEPAD_AXIS_LEFT_Y);
        std::cout << "Gamepad 1 X-axis: " << xAxisGamepad1 << ", Y-axis: " << yAxisGamepad1 << std::endl;

        float xAxisGamepad2 = inputSystem.getJoystickAxis(1, GLFW_GAMEPAD_AXIS_LEFT_X);
        float yAxisGamepad2 = inputSystem.getJoystickAxis(1, GLFW_GAMEPAD_AXIS_LEFT_Y);
        std::cout << "Gamepad 2 X-axis: " << xAxisGamepad2 << ", Y-axis: " << yAxisGamepad2 << std::endl;
*/
        if (inputSystem.GetGamepadButtonDown(0, GLFW_GAMEPAD_BUTTON_A)) {
            std::cout << "Gamepad 1 A button just got pressed." << std::endl;
        }

        if (inputSystem.GetGamepadButtonDown(1, GLFW_GAMEPAD_BUTTON_A)) {
            std::cout << "Gamepad 2 A button just got pressed." << std::endl;
        }

        movingObject.Move();
        //player1.Draw(shader);
        //floor.Draw(shader);
        movingObject.Draw();
        wagon.Draw();
        for(auto m: wagon._model.meshes) {
                spdlog::info(m.vertices.size());
        }

        walls.Draw();
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


        processInput(window);


        //camera

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


        glm:: mat4 view = camera.getView(movingObject);

        hitbox1.Draw(projection*view);
        floor.Draw(projection*view);

        //hitbox1.TestForIntersection(floor);

        //debugShape.DrawCube(glm::vec3(0), glm::vec3(1, 1, 1), glm::vec4(0), projection* view);
        shader.use();
        shader.setMat4("projectionView", projection * view);
        camera.followObject(movingObject);

        Engine::resolveCollisions();
        Engine::LoopEnd();

    }

    void ImGui() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            //player1.ImGui();
//            camera.ImGui();
//            pointLight.ImGui();
//            spotLight.ImGui();
//            dirLight.ImGui();
//
//            movingObject.ImGui();
//
////            floor.ImGui("Floor");
////            box.ImGui("Box");
//
//            hitbox1.ImGui("Player hitbox");
//            floor.ImGui("Floor hitbox");
//            hud.ImGui();
//            hud.imguiText();
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
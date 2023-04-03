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

namespace Game {

    void ImGui();

    const int SCR_WIDTH = 1000;
    const int SCR_HEIGHT = 800;


    GLFWwindow* window;

    Shader shader;

    Object3D player1;
    MovingObject movingObject;

    Camera camera(glm::vec3(0.f, 5.0f, 30.0f));

    Model test;

    DirectionalLight dirLight;
    SpotLight spotLight;
    PointLight pointLight;

    DebugShape debugShape;

    void Start() {
        std::cout << Engine::Init();

        debugShape.Initialize();

        window = Engine::getWindow();

        //player1.loadModel("../../res/models/first_character/first character.obj");
        movingObject.loadModel("../../res/models/first_character/first character.obj");


        Model tempM("../../res/models/first_character/first character.obj");
        test = tempM;

        // build and compile our shader zprogram
        // ------------------------------------
        Shader temp("../../res/shaders/shader.vert", "../../res/shaders/shader.frag");
        shader = temp;
        shader.use();

        // View and projection matricies
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -22.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        glm::mat4 viewProjection = projection * view;
        shader.setMat4("projectionView", viewProjection);

        glm::mat4 model = glm::mat4 (1.0f);
        shader.setMat4("model", model);

        DirectionalLight tempDirLight(glm::fvec3(0.05f, 0.05f, 0.05f), glm::fvec3(0.4f, 0.4f, 0.4f), glm::fvec3(0.5f, 0.5f, 0.5f), glm::fvec3(-0.2f, -1.0f, -0.3f));
        dirLight = tempDirLight;

        SpotLight tempSpotLight(glm::fvec3(0.05f, 0.05f, 0.05f), glm::fvec3(0.4f, 0.4f, 0.4f), glm::fvec3(0.5f, 0.5f, 0.5f), glm::fvec3(0.0f, 0.0f, 5.0f),
                                1.0f, 0.09f, 0.032f, glm::fvec3(0.0f, 0.0f, -1.0f), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)));
        spotLight = tempSpotLight;

        PointLight tempPointLight(glm::fvec3(0.05f, 0.05f, 0.05f), glm::fvec3(0.4f, 0.4f, 0.4f), glm::fvec3(0.5f, 0.5f, 0.5f), glm::fvec3(-3.2f, -3.0f, -3.3f),
                                1.0f, 0.09f, 0.032f);
        pointLight = tempPointLight;

        Parser parser("../../res/jsons");
        parser.saveJSON(dirLight.parseToJSON(), "dirLight");
        parser.saveJSON(spotLight.parseToJSON(), "spotLight");
        parser.saveJSON(pointLight.parseToJSON(), "pointLight");


        while (!glfwWindowShouldClose(window)) {
             Update();
        }
    }

    void Update() {
        Engine::LoopStart();
        ImGui();

        movingObject.Move();
        //player1.Draw(shader);
        movingObject.Draw(shader);

        ////REQUIRED FOR LIGHT
        //should be property of object
        shader.setInt("material.diffuse", 0);
        shader.setInt("material.specular", 1);
        shader.setFloat("material.shininess", 32.0f);
        //from camera
        shader.setVec3("viewPos", glm::vec3(0.0f, 0.0f, 1.0f));

        dirLight.sendToShader(shader, "dirLight");
        spotLight.sendToShader(shader, "spotLight");
        pointLight.sendToShader(shader, "pointLight");



        //camera

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


        glm:: mat4 view = camera.getView(camera.Look, camera.Position);


        debugShape.DrawCube(glm::vec3(0), glm::vec3(5, 5, 5), glm::vec4(0), projection* view);

        shader.setMat4("projectionView", projection * view);
        camera.findObject(movingObject);

        Engine::LoopEnd();

    }

    void ImGui() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            //player1.ImGui();
            camera.Imgui();
            pointLight.ImGui();
            spotLight.ImGui();
            dirLight.ImGui();

            movingObject.ImGui();
            Engine::ImGui();

        }
        ImGui::Render();
    }



};
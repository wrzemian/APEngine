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

//void processInput(GLFWwindow* window);

namespace Game {
    void processInput(GLFWwindow* window);

    void ImGui();

    const int SCR_WIDTH = 1000;
    const int SCR_HEIGHT = 800;

    GLFWwindow* window;

    Shader shader;

    Object3D player1;
    MovingObject movingObject;

    Camera camera(glm::vec3(0.f, 5.0f, 30.0f));
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    DirectionalLight dirLight;
    SpotLight spotLight;
    PointLight pointLight;

    InputSystem inputSystem;

    Hitbox hitbox1;
    //DebugShape debugShape;

    void Start() {
        std::cout << Engine::Init();

        window = Engine::getWindow();

        inputSystem.InputInit();
        inputSystem.monitorKey(GLFW_KEY_A);
        inputSystem.monitorKey(GLFW_KEY_SPACE);


        movingObject.loadModel("../../res/models/first_character/first character.obj");
        hitbox1.Create(&movingObject._transform, glm::vec3(1,3,2));

        // build and compile our shader program
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

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        inputSystem.update();

        if (inputSystem.GetKey(GLFW_KEY_A))
            std::cout << "escape1" << std::endl;
        if (inputSystem.GetKeyDown(GLFW_KEY_A))
            std::cout << "aaaa" << std::endl;

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

        dirLight.SendToShader(shader, "dirLight");
        spotLight.SendToShader(shader, "spotLight");
        pointLight.SendToShader(shader, "pointLight");

        processInput(window);

        //camera

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


        glm:: mat4 view = camera.getView(movingObject);

        hitbox1.Draw(projection*view);
        //debugShape.DrawCube(glm::vec3(0), glm::vec3(1, 1, 1), glm::vec4(0), projection* view);
        shader.use();
        shader.setMat4("projectionView", projection * view);
        camera.followObject(movingObject);

        Engine::LoopEnd();

    }

    void ImGui() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            //player1.ImGui();
            camera.ImGui();
            pointLight.ImGui();
            spotLight.ImGui();
            dirLight.ImGui();

            movingObject.ImGui();
            hitbox1.ImGui();

            Engine::ImGui();

        }
        ImGui::Render();
    }


    void processInput(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera.Position.y += 0.05;
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
            camera.Position.y -= 0.05;
    }

};
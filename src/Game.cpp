#include <iostream>


#include "../include/Game.h"

#include "../include/Engine.h"

#include "../include/Object3D.h"

#include "../include/lights/DirectionalLight.h"
#include "../include/Camera.h"


namespace Game {
    void ImGui();

    const int SCR_WIDTH = 1000;
    const int SCR_HEIGHT = 800;

    // Renderer - loads buffers and textures

    // ImGui sliders
    int depth = 0;

    glm::fvec3 color(1.0f, 1.0f, 0.0f);
    glm::fvec3 oldColor(1.0f, 1.0f, 1.0f);

    glm::fvec2 rotate(0.0f);
    glm::fvec2 oldRotate(1.0f);

    // Calculate translations for all pyramids across all depths
    Shader shader;

    Object3D player1;
    //Object3D player2;

    Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
    glm::vec3 position(camera.Position.x, camera.Position.y, camera.Position.z);

    Model test;

    DirectionalLight dirLight;
    glm::fvec3 direction(-0.2f, -1.0f, -0.3f);

    void Start() {
        std::cout << Engine::Init();

        //Model tempModel1("../../res/models/first_character/first character.obj");
        //Model tempModel2("../../res/models/second_character/second character.obj");

        player1.loadModel("../../res/models/first_character/first character.obj");
        //player2.loadModel("../../res/models/second_character/second character.obj");

        //player1._transform.addChild(&player2._transform);

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

        DirectionalLight tempDirLight(glm::fvec3(0.05f, 0.05f, 0.05f), glm::fvec3(0.4f, 0.4f, 0.4f), glm::fvec3(0.5f, 0.5f, 0.5f), direction);
        dirLight = tempDirLight;
    }

    void Update() {
        Engine::LoopStart();
        ImGui();
        //sierpinski.draw(&shader, depth);
        player1.Draw(shader);
        //player2.Draw(shader);
        test.Draw(shader);

        ////REQUIRED FOR LIGHT
        //should be property of object
        shader.setInt("material.diffuse", 0);
        shader.setInt("material.specular", 1);
        shader.setFloat("material.shininess", 32.0f);
        //from camera
        shader.setVec3("viewPos", glm::vec3(0.0f, 0.0f, 1.0f));


//        shader.setVec3("dirLight.direction", direction);
//        shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
//        shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
//        shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        dirLight.sendToShader(shader, "dirLight");

        //camera
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z),  // pozycja kamery
                                     glm::vec3(0.0f, 0.0f, 0.0f),  // punkt na który patrzy kamera
                                     glm::vec3(0.0f, 1.0f, 0.0f)); // wektor wskazujący kierunek "góry"

        shader.setMat4("projectionView", projection * view);


        Engine::LoopEnd();

    }

    void ImGui() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            player1.ImGui();
            camera.Imgui();
        }
        ImGui::Render();
    }



};
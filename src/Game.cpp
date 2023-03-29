#include <iostream>


#include "../include/Game.h"

#include "../include/Engine.h"

#include "../include/Object3D.h"



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

    //Object3D player1;
    //Object3D player2;

    Model test;

    void Start() {
        std::cout << Engine::Init();

        //Model tempModel1("../../res/models/first_character/first character.obj");
        //Model tempModel2("../../res/models/second_character/second character.obj");

        //player1.loadModel("../../res/models/first_character/first character.obj");
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



    }

    void Update() {
        Engine::LoopStart();
        ImGui();

        // send new rotation matrix to shader only when it changed
        if (rotate != oldRotate) {
            glm::mat4 rotation(1.0f);

            rotation = glm::rotate(rotation, glm::radians(rotate.x), glm::vec3(0.0f, 1.0f, 0.0f));
            rotation = glm::rotate(rotation, glm::radians(rotate.y), glm::vec3(1.0f, 0.0f, 0.0f));

            shader.setMat4("rotation", rotation);

            oldRotate = rotate;
        }
        // send new color vector to shader only when it changed
        if (color != oldColor) {
            shader.setVec3("uColor", color);
            oldColor = color;
        }

        //player1.Draw(shader);
        //player2.Draw(shader);
        test.Draw(shader);

        Engine::LoopEnd();

    }

    void ImGui() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
//            ImGui::Begin("Scale");
//            ImGui::SetWindowPos(ImVec2(0, 0));
//            ImGui::SetWindowSize(ImVec2(250, 100));
//
//            ImGui::SliderFloat("sX", &player1._transform._scale.x, 0.0f, 10.0f);
//            ImGui::SliderFloat("sY", &player1._transform._scale.y, 0.0f, 10.0f);
//            ImGui::SliderFloat("sZ", &player1._transform._scale.z, 0.0f, 10.0f);
//            ImGui::End();
//
//            ImGui::Begin("Position");
//            ImGui::SetWindowPos(ImVec2(0, 100));
//            ImGui::SetWindowSize(ImVec2(250, 100));
//
//            ImGui::SliderFloat("pX", &player1._transform._position.x, -10.0f, 10.0f);
//            ImGui::SliderFloat("pY", &player1._transform._position.y, -10.0f, 10.0f);
//            ImGui::SliderFloat("pZ", &player1._transform._position.z, -10.0f, 10.0f);
//
//            ImGui::End();
//
//
//            ImGui::Begin("Rotation");
//            ImGui::SetWindowPos(ImVec2(0, 200));
//            ImGui::SetWindowSize(ImVec2(250, 100));
//
//            ImGui::SliderFloat("rX", &player1._transform._rotation.x, -10.0f, 10.0f);
//            ImGui::SliderFloat("rY", &player1._transform._rotation.y, -10.0f, 10.0f);
//            ImGui::SliderFloat("rZ", &player1._transform._rotation.z, -10.0f, 10.0f);
//            ImGui::End();
        }
        ImGui::Render();
    }



};
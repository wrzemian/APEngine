#include <iostream>


#include "../include/Game.h"

#include "../include/Engine.h"

#include "../include/Model.h"
#include "../include/Transform.h"
#include "../include/Scene.h"

#include "../src/Renderer.cpp"


namespace Game {


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
    Sierpinski sierpinski;
    Shader shader;

    Renderer renderer;

    Model character1;
    Model character2;

    void Start() {
        std::cout << Engine::Init();

        Model tempModel1("../../res/models/first_character/first character.obj");
        character1 = tempModel1;

        Model tempModel2("../../res/models/second_character/second character.obj");
        character2 = tempModel2;

        renderer.print();
        renderer.loadBuffers();
        renderer.loadTexture("res/textures/stone.jpg");

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


        sierpinski.calculateAllTransformations();


    }

    void Update() {
        Engine::LoopStart();
        Engine::renderImGuiUI(&depth, &rotate, &color, sierpinski.MAX_DEPTH);

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
        sierpinski.draw(&shader, depth);

        character1.Draw(shader);
        character2.Draw(shader);

        Engine::LoopEnd();

    }




};
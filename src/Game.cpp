#include <iostream>


#include "../include/Game.h"

#include "../include/Engine.h"

#include "../include/Model.h"
#include "../include/Transform.h"
#include "../include/lights/DirectionalLight.h"
#include "../include/lights/SpotLight.h"
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

    DirectionalLight dirLight;
    glm::fvec3 ambient(0.05f, 0.05f, 0.05f);
    glm::fvec3 diffuse(0.4f, 0.4f, 0.4f);
    glm::fvec3 specular(0.5f, 0.5f, 0.5f);
    glm::fvec3 direction(-0.2f, -1.0f, -0.3f);

    SpotLight spotLight;
    glm::fvec3 ambientS(0.0f, 0.0f, 0.0f);
    glm::fvec3 diffuseS(1.0f, 1.0f, 1.0f);
    glm::fvec3 specularS(1.0f, 1.0f, 1.0f);
    glm::fvec3 position(0.0f, 0.0f, 1.0f);
    float constant(1.0f);
    float linear(0.09f);
    float quadratic(0.032f);
    glm::fvec3 directionS(0.0f, 0.0f, -1.0f);
    float cutOff(glm::cos(glm::radians(12.5f)));
    float outerCutOff(glm::cos(glm::radians(15.0f)));

    void SetLights(){
        dirLight.setAmbient(ambient);
        dirLight.setDiffuse(diffuse);
        dirLight.setSpecular(specular);
        dirLight.setDirection(direction);

        spotLight.setAmbient(ambientS);
        spotLight.setDiffuse(diffuseS);
        spotLight.setSpecular(specularS);
        spotLight.setPosition(position);
        spotLight.setConstant(constant);
        spotLight.setLinear(linear);
        spotLight.setQuadratic(quadratic);
        spotLight.setDirection(directionS);
        spotLight.setCutOff(cutOff);
        spotLight.setOuterCutOff(outerCutOff);
    }



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
        glm::mat4 model = glm::mat4 (1.0f);

        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -22.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        glm::mat4 viewProjection = projection * view;
        shader.setMat4("projectionView", viewProjection);
        shader.setMat4("model", model);

        sierpinski.calculateAllTransformations();

        DirectionalLight tempDirLight(ambient, diffuse, specular, direction);
        dirLight = tempDirLight;

        SpotLight tempSpotLight(ambientS, diffuseS, specularS, position, constant, linear, quadratic,
                                direction, cutOff, outerCutOff);
        spotLight = tempSpotLight;
    }

    void Update() {
        Engine::LoopStart();
        Engine::renderImGuiUI(&depth, &rotate, &color, sierpinski.MAX_DEPTH, &ambient, &diffuse, &specular, &direction,
                              &ambientS, &diffuseS, &specularS, &position, &constant, &linear, &quadratic, &directionS,
                              &cutOff, &outerCutOff);
        SetLights();
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

        //character1.Draw(shader);
        //character2.Draw(shader);

        ////REQUIRED FOR LIGHT
        //should be property of object
        shader.setInt("material.diffuse", 0);
        shader.setInt("material.specular", 1);
        shader.setFloat("material.shininess", 32.0f);
        //from camera
        shader.setVec3("viewPos", glm::vec3(0.0f, 0.0f, 1.0f));
        ////REQUIRED FOR LIGHT


        dirLight.sendToShader(shader, "dirLight");
        spotLight.sendToShader(shader, "spotLight");

        Engine::LoopEnd();

    }





};
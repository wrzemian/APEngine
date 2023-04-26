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
#include "../include/Animation.h"
#include "../include/Constant.h"

#include "spdlog/spdlog.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "al.h"
#include "alc.h"
#include "../include/Filesystem.h"

#include <filesystem>

namespace Game {
    void processInput();

    void ImGui();

    void renderScene(Shader shader);
    Shader simpleDepthShader;
    Shader debugDepthQuad;
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    unsigned int depthMap;
    float lightX = -1.0f;
    float lightY = 6.8f;
    float lightZ = 9.0f;
    glm::vec3 lightPos(lightX, lightY, lightZ);
    void renderQuad();
//    unsigned int woodTexture;
//    unsigned int loadTexture(char const * path);
    float near_plane = 0.1f, far_plane = 7.5f;

    float movImage = 0;
    HUD hud;
    Shader shader;
    Animation animation;
    Constant constant;
//    Image image;


    MovingObject player1;
    MovingObject player2;
    Walls wagon;

    Camera camera(glm::vec3(0.f, 5.0f, 30.0f));
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    GLfloat movementSpeed = 1.0f;


    DirectionalLight dirLight;
    SpotLight spotLight;
    PointLight pointLight;

    InputSystem inputSystem;


    void Start() {
        std::cout << Engine::Init() <<"\n";

        ALCdevice *device;
        device = alcOpenDevice(NULL);

        inputSystem.InputInit();
        inputSystem.monitorKey(GLFW_KEY_W);
        inputSystem.monitorKey(GLFW_KEY_A);
        inputSystem.monitorKey(GLFW_KEY_S);
        inputSystem.monitorKey(GLFW_KEY_D);
        inputSystem.monitorKey(GLFW_KEY_UP);
        inputSystem.monitorKey(GLFW_KEY_LEFT);
        inputSystem.monitorKey(GLFW_KEY_RIGHT);
        inputSystem.monitorKey(GLFW_KEY_DOWN);
        inputSystem.monitorKey(GLFW_KEY_SPACE);
        inputSystem.monitorKey(GLFW_KEY_KP_1);


//        hud.initAnimation();
        //hud.initImage("res/textures/tlo.png");
        hud.initText("res/fonts/Arialn.ttf");

        player1.loadFromJSON(Engine::parser.CreateFromJSONMovingObject("movingObj_0"));
        player2.loadFromJSON(Engine::parser.CreateFromJSONMovingObject("movingObj_1"));

        wagon.loadFromJSON(Engine::parser.CreateFromJSONWalls("walls"));
//        wagon.loadModel("../../res/models/1level/1level.obj");
//        wagon.calculateHitboxes();
//        wagon.logHitboxes();
//        for(auto m: wagon._model.meshes) {
//            spdlog::info("loaded mesh with {} vertices", m.vertices.size());
//        }

        // build and compile our shader program
        // ------------------------------------
        Shader temp("../../res/shaders/shadowShader.vert", "../../res/shaders/shadowShader.frag");
        shader = temp;

//        woodTexture = loadTexture(FileSystem::getPath("resources/textures/tlo.png").c_str());


        shader.setMat4("projectionView", camera.viewProjection);

        glm::mat4 model = glm::mat4 (1.0f);
        shader.setMat4("model", model);


        dirLight = Engine::parser.CreateFromJSONDir("dirLight");
        spotLight = Engine::parser.CreateFromJSONSpot("spotLight");
        pointLight = Engine::parser.CreateFromJSONPoint("pointLight");

        spdlog::info("ImGui");
        ImGui();
        spdlog::info("ResolveCollisions");
        Engine::resolveCollisions();
        spdlog::info("loop");


        Shader animationShader("../../res/shaders/animationsShader.vert", "../../res/shaders/animationsShader.frag");
        Animation animation1(animationShader);
        animation = animation1;
        animation.initAnimation();

        Constant constant1(animationShader);
        constant = constant1;
        constant.initConstant();



        Shader tempSimpleDepthShader("../../res/shaders/depthShader.vert", "../../res/shaders/depthShader.frag");
        simpleDepthShader = tempSimpleDepthShader;

        Shader tempDebugDepthQuad("../../res/shaders/debugShadow.vert", "../../res/shaders/debugShadow.frag");
        debugDepthQuad = tempDebugDepthQuad;


        glGenFramebuffers(1, &depthMapFBO);
        // create depth texture

        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        shader.use();
        shader.setInt("diffuseTexture", 0);
        shader.setInt("shadowMap", 1);
        debugDepthQuad.use();
        debugDepthQuad.setInt("depthMap", 0);

        while (!glfwWindowShouldClose(Engine::getWindow())) {
            Update();
        }
    }

    void Update() {
        Engine::LoopStart();
        ImGui();
        lightPos = glm::vec3(lightX, lightY, lightZ);

        inputSystem.update();
        processInput();
        movImage -= 0.1;

        float time = static_cast<float>(glfwGetTime());
        animation.renderAnimation(time);
        constant.renderConstant();

        //player1.Move();

        Engine::moveObjects();

        // 1. render depth of scene to texture (from light's perspective)
        // --------------------------------------------------------------
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;

        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        // render scene from light's point of view
        simpleDepthShader.use();
        simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, woodTexture);
        renderScene(simpleDepthShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // reset viewport
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 2. render scene as normal using the generated depth/shadow map
        // --------------------------------------------------------------
        shader.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        // set light uniforms
        shader.setVec3("viewPos", camera.Position);
        shader.setVec3("lightPos", lightPos);
        shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, woodTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        renderScene(shader);

        // render Depth map to quad for visual debugging
        // ---------------------------------------------
//        debugDepthQuad.use();
//        debugDepthQuad.setFloat("near_plane", near_plane);
//        debugDepthQuad.setFloat("far_plane", far_plane);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, depthMap);
//        renderQuad();


        //player1.Draw();
        //wagon.Draw();

        //box.Draw(shader);

        ////REQUIRED FOR LIGHT
        //should be property of object
//        shader.setInt("material.diffuse", 0);
//        shader.setInt("material.specular", 1);
//        shader.setFloat("material.shininess", 32.0f);
//        //from camera
//        shader.setVec3("viewPos", glm::vec3(0.0f, 0.0f, 1.0f));
//
//        Engine::renderLights(shader);






        //camera

//        glm::mat4 projection = glm::mat4(1.0f);
//        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//
//        glm:: mat4 view = camera.getView(player1);

//        shader.use();
//        shader.setMat4("projectionView", projection * view);

        Engine::renderHitboxes(projection * view);

        camera.followObject(player1);

        Engine::resolveCollisions();

        hud.renderText("nie psuje textur?",100,0,2,glm::vec3(1.0f, 1.0f, 1.0f));

        Engine::LoopEnd();

    }

    void ImGui() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            Engine::renderImgui();
            Engine::ImGui();

            ImGui::Begin("LIGHTPOS FOR SHADOW");
            ImGui::SetWindowSize(ImVec2(250, 150));

            ImGui::SliderFloat("lightX", &lightX, -50.0f, 50.0f);
            ImGui::SliderFloat("lightY", &lightY, -50.0f, 50.0f);
            ImGui::SliderFloat("lightZ", &lightZ, -50.0f, 50.0f);

            ImGui::End();
        }
        ImGui::Render();
    }

    void renderScene(Shader shader)
    {
        player1.setShader(&shader);
        player2.setShader(&shader);
        wagon.setShader(&shader);
        Engine::drawObjects();
    }

    void processInput()
    {
        if (inputSystem.GetKey(GLFW_KEY_W))
            player1._transform._position.z += -movementSpeed;
        if (inputSystem.GetKey(GLFW_KEY_A))
            player1._transform._position.x += -movementSpeed;
        if (inputSystem.GetKey(GLFW_KEY_S))
            player1._transform._position.z += movementSpeed;
        if (inputSystem.GetKey(GLFW_KEY_D))
            player1._transform._position.x += movementSpeed;
        if (inputSystem.GetKey(GLFW_KEY_SPACE))
            player1.AddVelocity(glm::vec3(0.0f, 1.0f, 0.0f));
        if (inputSystem.GetKey(GLFW_KEY_UP))
            player2._transform._position.z += -movementSpeed;
        if (inputSystem.GetKey(GLFW_KEY_LEFT))
            player2._transform._position.x += -movementSpeed;
        if (inputSystem.GetKey(GLFW_KEY_DOWN))
            player2._transform._position.z += movementSpeed;
        if (inputSystem.GetKey(GLFW_KEY_RIGHT))
            player2._transform._position.x += movementSpeed;
        if (inputSystem.GetKey(GLFW_KEY_KP_1))
            player2.AddVelocity(glm::vec3(0.0f, 1.0f, 0.0f));

        player1.SetVelocity(glm::vec3(inputSystem.getJoystickAxis(0, GLFW_GAMEPAD_AXIS_LEFT_X), player1._velocity.y, inputSystem.getJoystickAxis(0, GLFW_GAMEPAD_AXIS_LEFT_Y)));
        player2.SetVelocity(glm::vec3(inputSystem.getJoystickAxis(1, GLFW_GAMEPAD_AXIS_LEFT_X),player2._velocity.y,inputSystem.getJoystickAxis(1, GLFW_GAMEPAD_AXIS_LEFT_Y)));
    }

    // renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
    unsigned int quadVAO = 0;
    unsigned int quadVBO;
    void renderQuad()
    {
        if (quadVAO == 0)
        {
            float quadVertices[] = {
                    // positions        // texture Coords
                    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                    1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };
            // setup plane VAO
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        }
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }

    unsigned int loadTexture(char const * path)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }

};
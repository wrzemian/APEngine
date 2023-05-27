#include <iostream>


#include "../include/Game.h"

#include "../include/Engine.h"

#include "../include/Objects/Object3D.h"
#include "../include/Objects/MovingObject.h"

#include "../include/lights/DirectionalLight.h"
#include "../include/Camera.h"
#include "../include/Objects/DebugShape.h"

#include "../include/lights/SpotLight.h"
#include "../include/lights/PointLight.h"

#include "../include/Parser.h"
#include "../include/Objects/Hitbox.h"
#include "../include/User/InputSystem.h"
#include "../include/User/HUD.h"
#include "../include/Objects/Walls.h"
#include "../include/User/Animation.h"
#include "../include/User/Constant.h"
#include "../include/Objects/Platform.h"
#include "../include/Objects/Button.h"
#include "../include/Objects/PlayerJumper.h"
#include "../include/Objects/PlayerGrabber.h"
#include "../include/Objects/Battery.h"

//temporary
#include "../include/Background/Rock.h"
#include "../include/Background/Cactus.h"
#include "../include/Background/BackgroundTile.h"
#include "../include/Background/Background.h"

#include "../include/AssetManager.h"

#include "spdlog/spdlog.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "al.h"
#include "alc.h"
#include "../include/Objects/SimpleHitbox.h"


namespace Game {
    void processInput();
    float imgMOv=0;
    void ImGui();

    void renderScene(Shader shader, const Camera& camera);
    Shader simpleDepthShader;
    Shader debugDepthQuad;
    const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
    unsigned int depthMapFBO;
    unsigned int depthMap;
    float lightX = -5.0f;
    float lightY = 11.370;
    float lightZ = 15.130f;
    glm::vec3 lightPos(lightX, lightY, lightZ);
    void renderQuad();
//    unsigned int woodTexture;
//    unsigned int loadTexture(char const * path);
    float near_plane = 0.1f, far_plane = 30.5f;

    float movImage = 0;
    HUD hud;
    HUD hud2;
    Shader shader;
    Animation animation;
    Constant constant;
//    Image image;


    //MovingObject player1;
    PlayerJumper playerJumper;
    PlayerGrabber playerGrabber;
    //MovingObject player2;
    Ant ant;

    Camera camera;

    Hitbox p1Hitbox("hitboxes/hitbox_0");
    Hitbox p2Hitbox("hitboxes/hitbox_1");
    Hitbox antHitbox("hitboxes/hitbox_2");
    SimpleHitbox antBigHitbox("hitboxes/hitbox_3");

    Hitbox platform1Hitbox("hitboxes/hitbox_platform");
    Hitbox button1Hitbox("hitboxes/hitbox_button");

    Battery battery;
    Hitbox batteryHitbox("hitboxes/hitbox_battery");

    Walls wagon;


    GLfloat movementSpeed = 3.0f;


    DirectionalLight dirLight;
    SpotLight spotLight;
    PointLight pointLight;

    InputSystem inputSystem;

    Background background;

    Platform platform(glm::vec3(-8,6,5),glm::vec3(-8,9,5),1.0f);

    Button button(&platform,glm::vec3(-8,5,7));

    void Start() {
        std::cout << Engine::Init() <<"\n";

        camera.ShowImgui();;

        inputSystem.InputInit();
        /*inputSystem.monitorKey(GLFW_KEY_W);
        inputSystem.monitorKey(GLFW_KEY_A);
        inputSystem.monitorKey(GLFW_KEY_S);
        inputSystem.monitorKey(GLFW_KEY_D);*/
        inputSystem.monitorKey(GLFW_KEY_SPACE);
        inputSystem.monitorKey(GLFW_KEY_KP_1);

        playerJumper.initPlayer(&inputSystem);
        playerGrabber.initPlayer(&inputSystem);

//        hud.initAnimation();
//        hud.initImage("res/textures/tlo.png");
        hud2.initText("res/fonts/Arialn.ttf");


        p2Hitbox.draw = true;
        p1Hitbox.draw = true;
        antBigHitbox.draw = true;


        //player1.loadFromJSON(Engine::parser.CreateFromJSONMovingObject("objects/movingObj_0"));
        //player2.loadFromJSON(Engine::parser.CreateFromJSONMovingObject("objects/movingObj_1"));
        //player1.tag = "player";
        //player2.tag = "player";

        ant.loadFromJSON(Engine::parser.CreateFromJSONMovingObject("objects/movingObj_2"));
        ant.tag = "ant";



        camera = Engine::parser.CreateFromJSONCam("camera");

        p1Hitbox.Create(&playerJumper);
        p2Hitbox.Create(&playerGrabber);

        //p1Hitbox.calculateFromModel(player1._model);
        //p2Hitbox.calculateFromModel(player2._model);
        wagon.loadFromJSON(Engine::parser.CreateFromJSONWalls("objects/walls"));
        //wagon.setShader(&shader);

        // build and compile our shader program
        // ------------------------------------
        Shader temp("../../res/shaders/shadows/shadowShader.vert", "../../res/shaders/shadows/shadowShader.frag");
        shader = temp;

        //player1.setShader(&shader);
        //playerJumper.setShader(&shader);
        //playerGrabber.setShader(&shader);
        //player2.setShader(&shader);
        //ant.setShader(&shader);
        //wagon.setShader(&shader);

        platform.loadModel("../../res/models/Assets/chest1/box1.obj");
        //platform.setShader(&shader);
        platform1Hitbox.Create(&platform);
        platform1Hitbox.draw = true;

        //button.setShader(&shader);
        button.loadModel("../../res/models/Assets/chest1/box1.obj");
        button1Hitbox.Create(&button);
        button1Hitbox.isTrigger = true;

        //background.initBackground(5,-525.509948,262.754974,&shader);

        //battery.setShader(&shader);
        battery.loadModel("../../res/models/Assets/battery/battery.obj");
        battery.tag = "battery";
        batteryHitbox.Create(&battery);
        batteryHitbox.draw = true;
        battery._transform._position.x = -8;
        battery._transform._position.y = 7;
        battery._transform._position.z = 6;
        battery._transform._scale.x = 0.2f;
        battery._transform._scale.y = 0.2f;
        battery._transform._scale.z = 0.2f;
        playerJumper.battery = &battery;
        playerGrabber.battery = &battery;


        shader.setMat4("projectionView", camera.getViewProjection());

        glm::mat4 model = glm::mat4 (1.0f);
        shader.setMat4("model", model);

        dirLight = Engine::parser.CreateFromJSONDir("lights/dirLight");
        spotLight = Engine::parser.CreateFromJSONSpot("lights/spotLight");
        pointLight = Engine::parser.CreateFromJSONPoint("lights/pointLight");

        spdlog::info("ImGui");
        ImGui();
        spdlog::info("ResolveCollisions");
        Engine::resolveCollisions();
        //Engine::logStaticHitboxes();
        //Engine::logDynamicHitboxes();
        spdlog::info("loop");


        Shader animationShader("../../res/shaders/animationsShader.vert", "../../res/shaders/animationsShader.frag");
        Animation animation1(animationShader);
        animation = animation1;
        animation.initAnimation();

        Constant constant1(animationShader);
        constant = constant1;
        constant.initConstant();

        Shader imageShader("../../res/shaders/imageShader.vert", "../../res/shaders/imageShader.frag");
        HUD hud1(imageShader);
        hud = hud1;
        hud.initImage("res/textures/tlo.png");

        Shader tempSimpleDepthShader("../../res/shaders/shadows/depthShader.vert", "../../res/shaders/shadows/depthShader.frag");
        simpleDepthShader = tempSimpleDepthShader;

        Shader tempDebugDepthQuad("../../res/shaders/shadows/debugShadow.vert", "../../res/shaders/shadows/debugShadow.frag");
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
        lightPos = glm::vec3(lightX, lightY, lightZ);

        Engine::LoopStart();
        ImGui();
        imgMOv -= 0.1;
        inputSystem.update();
        processInput();
        playerJumper.UpdatePlayer(&inputSystem,movementSpeed);
        playerGrabber.UpdatePlayer(&inputSystem,movementSpeed);
        movImage -= 0.1;

        float time = static_cast<float>(glfwGetTime());
        animation.renderAnimation(time,2,520,1);
        constant.renderConstant(2,550,1);
        hud.renderImage(imgMOv);

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
        renderScene(simpleDepthShader, camera);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // reset viewport
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//        // SHADOW MAP DEBUGGING
//        //---------------------------------------------
//        debugDepthQuad.use();
//        debugDepthQuad.setFloat("near_plane", near_plane);
//        debugDepthQuad.setFloat("far_plane", far_plane);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, depthMap);
//        renderQuad();

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
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        renderScene(shader, camera);

        //player1.Draw();
        //wagon.Draw();

        //box.Draw(shader);

//        ////REQUIRED FOR LIGHT
//        //should be property of object
//        shader.setInt("material.diffuse", 0);
//        shader.setInt("material.specular", 1);
//        shader.setFloat("material.shininess", 32.0f);
//        //from camera
//        shader.setVec3("viewPos", camera.Position);

        //Engine::renderLights(shader);



        //background.Move(-Engine::deltaTime*40);
        platform.UpdatePosition(Engine::deltaTime);

        //camera

        //glm::mat4 projection = glm::mat4(1.0f);
//        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)Engine::SCR_WIDTH / (float)Engine::SCR_HEIGHT, 0.1f, 100.0f);
//        glm::mat4 view = camera.GetViewMatrix();
//
//        shader.use();
//        shader.setMat4("projectionView", projection * view);

        //Engine::renderHitboxes(projection * view);

//       camera.followObject(player1);
        button.Update(Engine::deltaTime);
        Engine::resolveCollisions();

        hud2.renderText("nie psuje textur?",100,0,2,glm::vec3(1.0f, 1.0f, 1.0f));

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

            //playerJumper.ShowImgui();
            //playerGrabber.ShowImgui();
            camera.ShowImgui();
        }
        ImGui::Render();
    }


    void processInput()
    {
        /*
        if (inputSystem.GetKey(GLFW_KEY_W)) {
            player1._velocity.z = -movementSpeed;
        }
        else if (inputSystem.GetKey(GLFW_KEY_S)) {
            player1._velocity.z = movementSpeed;
        }
        else {
            player1._velocity.z = 0;
        }
        if (inputSystem.GetKey(GLFW_KEY_A)) {
            player1._velocity.x = -movementSpeed;
        }
        else if (inputSystem.GetKey(GLFW_KEY_D)) {
            player1._velocity.x = movementSpeed;
        }
        else {
            player1._velocity.x = 0;
        }*/
        if (inputSystem.GetKeyDown(GLFW_KEY_SPACE)) {
            playerJumper.Jump();
        }

/*
        if (inputSystem.GetKey(GLFW_KEY_UP)) {
            player2._velocity.z = -movementSpeed;
        }
        else if (inputSystem.GetKey(GLFW_KEY_DOWN)) {
            player2._velocity.z = movementSpeed;
        }
        else {
            player2._velocity.z = 0;
        }

        if (inputSystem.GetKey(GLFW_KEY_LEFT)) {
            player2._velocity.x = -movementSpeed;
        }
        else if (inputSystem.GetKey(GLFW_KEY_RIGHT)) {
            player2._velocity.x = movementSpeed;
        }
        else {
            player2._velocity.x = 0;
        }*/
        if (inputSystem.GetKeyDown(GLFW_KEY_KP_1)) {
            playerGrabber.AddVelocity(glm::vec3(0.0f, 5.0f, 0.0f));
        }

//        player1.SetVelocity(glm::vec3(inputSystem.getJoystickAxis(0, GLFWD_GAMEPAD_AXIS_LEFT_X), player1._velocity.y, inputSystem.getJoystickAxis(0, GLFW_GAMEPAD_AXIS_LEFT_Y)));
//        player2.SetVelocity(glm::vec3(inputSystem.getJoystickAxis(1, GLFW_GAMEPAD_AXIS_LEFT_X),player2._velocity.y,inputSystem.getJoystickAxis(1, GLFW_GAMEPAD_AXIS_LEFT_Y)));
    }

    void renderScene(Shader shader, const Camera& camera)
    {
        playerJumper.setShader(&shader);
        playerGrabber.setShader(&shader);
        ant.setShader(&shader);
        wagon.setShader(&shader);
        platform.setShader(&shader);
        button.setShader(&shader);
        battery.setShader(&shader);
        Engine::drawObjects(camera);
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

};
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
#include "../include/Objects/Level.h"
#include "../include/User/HudAnimation.h"
#include "../include/User/Constant.h"
#include "../include/Objects/Platform.h"
#include "../include/Objects/Button.h"
#include "../include/Objects/PlayerJumper.h"
#include "../include/Objects/PlayerGrabber.h"
#include "../include/Objects/Battery.h"
#include "../include/Objects/Grabber.h"
#include "../include/Objects/Box.h"
#include "../include/Objects/WinArea.h"
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
#include "../include/Animations2/Animator.h"
#include "../include/lights/Shadows.h"
#include "../include/LevelManager.h"
#include "../include/Audio/AudioManager.h"

namespace Game {
    void processInput();

    float imgMOv = 0;

    void ImGui();

    void renderScene(Shader shader, const Camera &camera);

    Shader simpleDepthShader;
    Shader debugDepthQuad;


    float movImage = 0;
    HUD hud;
    HUD hud2;
    Shader shader;
    HudAnimation animation;
    Shadows shadows("lights/shadows");
    Shader lightShader;
//    Animation animation;
    Constant constant;
//    Image image;


    PlayerJumper playerJumper;
    PlayerGrabber playerGrabber;

    Ant ant;

    Camera camera("camera");

    //animation testing

    Hitbox p1Hitbox("hitboxes/hitbox_0");
    Hitbox p2Hitbox("hitboxes/hitbox_1");
    Hitbox antHitbox("hitboxes/hitbox_2");
    SimpleHitbox antBigHitbox("hitboxes/hitbox_3");

//    Battery battery;
//    Hitbox batteryHitbox("hitboxes/hitbox_battery");

    Grabber grabber;
    Hitbox grabberHitbox("hitboxes/hitbox_grabber");

   // Box box;
   // Hitbox boxHitbox("hitboxes/hitbox_box1");

//    WinArea winArea;
//    Hitbox winHitbox("hitboxes/hitbox_win");


    Level* currentLevel;

    GLfloat movementSpeed = 3.0f;


    DirectionalLight dirLight;
    SpotLight spotLight;
    PointLight pointLight;

    InputSystem inputSystem;

    std::string texToDisplay = "";

    Background background;



    void Start() {
        Engine::Init();
        spdlog::info("init engine");


        spdlog::info("init audio");
        AudioManager::GetInstance()->InitializeAudio();
        AudioManager::GetInstance()->CreateAll(camera, playerGrabber);



        LevelManager::getInstance().loadAllLevels("../../res/models/Levels/levelList");
        LevelManager::getInstance().loadAllLevelsData("../../res/jsons/levels/levelList");
        LevelManager::getInstance().ShowImgui();


        shadows.initShaders();
        spdlog::info("shader");
        Shader tempLightShader("../../res/shaders/shader.vert", "../../res/shaders/shader.frag");
        lightShader = tempLightShader;
        lightShader.use();

        inputSystem.InputInit();
        inputSystem.monitorKey(GLFW_KEY_SPACE);
        inputSystem.monitorKey(GLFW_KEY_KP_1);

        spdlog::info("init jumper");
        playerJumper.initPlayer(&inputSystem);
        spdlog::info("init grabber");
        playerGrabber.initPlayer(&inputSystem);

        playerGrabber.levelId = 0;
        playerJumper.levelId = 0;
        grabber.levelId = 0;

//        playerGrabber.loadAnimation("res/models/Players/Cr4nk/crank_jumping_final.dae");
//        playerGrabber.loadAnimation("res/models/Players/Cr4nk/crank_movement_final.dae");
//        playerJumper.loadAnimation("res/models/Players/Mich3l/animation/michel_movement.dae");
//        Animation temp(daePath, &*_model);


//        hud.initAnimation();
//        hud.initImage("res/textures/tlo.png");
        spdlog::info("init text");
        hud2.initText("res/fonts/Arialn.ttf");


        p2Hitbox.draw = false;
        p1Hitbox.draw = false;
        antBigHitbox.draw = false;


        //player1.loadFromJSON(Engine::parser.CreateFromJSONMovingObject("objects/movingObj_0"));
        //player2.loadFromJSON(Engine::parser.CreateFromJSONMovingObject("objects/movingObj_1"));
        //player1.tag = "player";
        //player2.tag = "player";

        spdlog::info("loading ant");
        ant.loadFromJSON(Engine::parser.CreateFromJSONMovingObject("objects/movingObj_2"));
        ant.tag = "ant";


        //camera = Engine::parser.CreateFromJSONCam("camera");
        camera.ShowImgui();;

        //p1Hitbox.Create(&playerJumper);
        //p2Hitbox.Create(&playerGrabber);

        //p1Hitbox.calculateFromModel(player1._model);
        //p2Hitbox.calculateFromModel(player2._model);



        //currentLevel = LevelManager.getInstance().

        background.initBackground(5,-420.4079584,210.2039792,&shader);//-525.509948


//        battery.loadModel("../../res/models/Assets/battery/battery.obj");
//        battery.tag = "battery";
//        //batteryHitbox.Create(&battery);
//        batteryHitbox.draw = true;
//        battery._transform._position.x = -8;
//        battery._transform._position.y = 7;
//        battery._transform._position.z = 6;
//        battery._transform._scale.x = 0.2f;
//        battery._transform._scale.y = 0.2f;
//        battery._transform._scale.z = 0.2f;
        if(!LevelManager::getInstance().getCurrentLevel()->batteries.empty()) { // TODO: fix this tomfoolery
            playerJumper.battery = LevelManager::getInstance().getCurrentLevel()->batteries.at(0).get();
            playerGrabber.battery = LevelManager::getInstance().getCurrentLevel()->batteries.at(0).get();
        }


        playerGrabber.grabber = &grabber;
        grabber.loadModel("../../res/models/Assets/battery/battery.obj");
        grabberHitbox.draw = false;
        grabberHitbox.isTrigger = true;


//        box.setShader(&shader);
//        box.loadModel("../../res/models/Assets/chest1/box1.obj");
//        box._transform._position.x = -4.2f;
//        box._transform._position.y = 8.5f;
//        box._transform._position.z = 8.0f;
//        box._transform._scale.x=0.5f;
//        box._transform._scale.z=0.5f;
//        box.ShowImgui();
//        //boxHitbox.draw = true;
//        boxHitbox.ShowImgui();

//        winArea.setShader(&shader);
//        winArea.loadModel("../../res/models/Assets/chest1/box1.obj");
//        winArea.rendered = false;
//        winArea._transform._position.x = -1.5f;
//        winArea._transform._position.y = 8.0f;
//        winArea._transform._position.z = 8.0f;
//        winArea.ShowImgui();
//        winHitbox.draw = false;
//        winHitbox.ShowImgui();
//        winHitbox.isTrigger = true;
//        winArea.text = &texToDisplay;

        shader.setMat4("projectionView", camera.getViewProjection());

        glm::mat4 model = glm::mat4(1.0f);
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
        HudAnimation animation1(animationShader);
        animation = animation1;
        animation.initAnimation();

        Constant constant1(animationShader);
        constant = constant1;
        constant.initConstant();

        Shader imageShader("../../res/shaders/imageShader.vert", "../../res/shaders/imageShader.frag");
        HUD hud1(imageShader);
        hud = hud1;
        hud.initImage("res/textures/tlo.png");


//        Model ourModel("include/Animations2/testing/first_character.dae");
//        Animation danceAnimation("include/Animations2/testing/first_character.dae",
//                                 &ourModel);
//        Animator animator(&danceAnimation);



      //  Engine::logTextures();
        while (!glfwWindowShouldClose(Engine::getWindow())) {
            Update();
        }
    }

    void Update() {

        Engine::LoopStart();
//        std::this_thread::sleep_for(std::chrono::duration<double>(1.0 / 30.0)); // to slow down frame rate for fewer collisions detection

        AudioManager::GetInstance()->Update();

        ImGui();
        imgMOv -= 0.1f;
        inputSystem.update();
        processInput();
        playerJumper.UpdatePlayer(&inputSystem, movementSpeed);
        playerGrabber.UpdatePlayer(&inputSystem, movementSpeed);
        movImage -= 0.1;

        float time = static_cast<float>(glfwGetTime());
        animation.renderAnimation(time, 2, 520, 1);
        constant.renderConstant(2, 550, 1);
        hud.renderImage(imgMOv);

        //player1.Move();

        Engine::moveObjects();

        //Engine::renderLights(lightShader, camera);
        shadows.renderShadows(camera);
        camera.Update(Engine::deltaTime);
        //glm::mat4 projection = glm::mat4(1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)Engine::SCR_WIDTH / (float)Engine::SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
//
//        shader.use();
//        shader.setMat4("projectionView", projection * view);

        background.Move(-50*Engine::deltaTime);


        Engine::renderHitboxes(projection * view);

//       camera.followObject(player1);
        Engine::resolveCollisions();

        hud2.renderText(texToDisplay, 100, 0, 2, glm::vec3(1.0f, 1.0f, 1.0f));

        Engine::LoopEnd();

    }

    void onWin() {
        if(LevelManager::getInstance().nextLevel())
        {
            playerGrabber.levelId = LevelManager::getInstance().currentLevel;
            playerJumper.levelId = LevelManager::getInstance().currentLevel;
            grabber.levelId = LevelManager::getInstance().currentLevel;

            playerGrabber._transform._position = LevelManager::getInstance().getCurrentLevel()->playerGrabberStartingPos; //+ LevelManager::getInstance().getCurrentLevel()->_transform._position;
            playerJumper._transform._position = LevelManager::getInstance().getCurrentLevel()->playerJumperStartingPos; //+ LevelManager::getInstance().getCurrentLevel()->_transform._position;


            camera.MoveToTarget( LevelManager::getInstance().getCurrentLevel()->cameraOffset);
            if(!LevelManager::getInstance().getCurrentLevel()->batteries.empty())
            {
                playerJumper.battery = LevelManager::getInstance().getCurrentLevel()->batteries.at(0).get();
                playerGrabber.battery = LevelManager::getInstance().getCurrentLevel()->batteries.at(0).get();
            }
            spdlog::info("Player has won the level!");
        }
        else
        {
            spdlog::info("Player has won the game!");
        }
    }


    void ImGui() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            Engine::renderImgui();
            Engine::ImGui();
            dirLight.ShowImgui();
            pointLight.ShowImgui();
            spotLight.ShowImgui();
            camera.ShowImgui();
            shadows.ShowImgui();
            playerGrabber.ShowImgui();
        }


        ImGui::Begin("DevTools");
        ImGui::SetWindowSize(ImVec2(250, 100));
        if (ImGui::Button("Next Level"))
            onWin();
        ImGui::End();


        ImGui::Render();
    }


    void processInput() {
        if (inputSystem.GetKeyDown(GLFW_KEY_SPACE) || inputSystem.GetGamepadButtonDown(1, GLFW_GAMEPAD_BUTTON_A)) {
            playerJumper.Jump();
        }
        if (inputSystem.GetKeyDown(GLFW_KEY_KP_1) || inputSystem.GetGamepadButtonDown(0, GLFW_GAMEPAD_BUTTON_A)) {
            playerGrabber.Jump();
            playerGrabber.switchAnimationJump();
            AudioManager::GetInstance()->PlaySound(Audio::JUMP);
        }
    }

};
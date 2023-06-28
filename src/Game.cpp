#include <iostream>


#include "../include/Game.h"

#include "../include/Engine.h"

#include "../include/Objects/Object3D.h"
#include "../include/Objects/MovingObject.h"

#include "../include/lights/DirectionalLight.h"
#include "../include/camera.h"
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
#include "../include/User/SpriteRenderer.h"
#include "../include/User/Menu.h"

namespace Game {
    void processInput();

    void ImGui();

    void renderScene(Shader shader, Camera camera);

    Shader simpleDepthShader;
    Shader debugDepthQuad;
    Shader spriteShader;

    SpriteRenderer jumpUI;

    HUD hud;
    HUD hud2;
    Menu menu;
    Menu menuPause;
    SpriteRenderer endingScreen;
    Shader shader;
    HudAnimation animation;
    Shadows shadows("lights/shadows");
    Shader lightShader;
    Constant constant;

    PlayerGrabber playerGrabber;
    PlayerJumper playerJumper;

    Hitbox p1Hitbox("hitboxes/hitbox_0");
    Hitbox p2Hitbox("hitboxes/hitbox_1");

    Grabber grabber;
    Hitbox grabberHitbox("hitboxes/hitbox_grabber");

    Level* currentLevel;

    GLfloat movementSpeed = 3.0f;

    SpriteRenderer  UITips[3];

    DirectionalLight dirLight;

    InputSystem inputSystem;

    std::string texToDisplay = "";

    Background background;

    float bgSpeed = -50.0f;
    bool slowdown = false;


    void Start() {
        spdlog::info("init engine");
        Engine::Init();

        spdlog::info("init audio");
        AudioManager::GetInstance()->InitializeAudio();
        AudioManager::GetInstance()->CreateAll(*Engine::camera, playerGrabber);

        LevelManager::getInstance().loadAllLevels("../../res/models/Levels/levelList");
        LevelManager::getInstance().loadAllLevelsData("../../res/jsons/levels/levelList");
        LevelManager::getInstance().ShowImgui();

        shadows.initShaders(*Engine::camera);
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


        playerJumper.loadAnimation("res/models/Players/Mich3l/michel_breathing_and_looking_around.dae");
        playerGrabber.loadAnimation("res/models/Players/Cr4nk/REST_CRANK_WALKING.dae");

        spdlog::info("init text");
        hud2.initText("res/fonts/Arialn.ttf");

        p1Hitbox.tag = "player";
        p2Hitbox.tag = "player";

        p1Hitbox.ShowImgui();
        p2Hitbox.ShowImgui();

        Engine::camera->ShowImgui();

        playerGrabber.grabber = &grabber;
        grabber.loadModel("res/models/Players/Cr4nk/RIGHT_HAND_CRANK_HOOKING.dae");
        grabberHitbox.isTrigger = true;

        background.initBackground(5,-420.4079584,210.2039792,&shader);//-525.509948

        if(!LevelManager::getInstance().getCurrentLevel()->batteries.empty()) { // TODO: fix this tomfoolery
            playerJumper.battery = LevelManager::getInstance().getCurrentLevel()->batteries.at(0).get();
            playerGrabber.battery = LevelManager::getInstance().getCurrentLevel()->batteries.at(0).get();
        }


        shader.setMat4("projectionView", Engine::camera->getViewProjection());

        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", model);

        dirLight = Engine::parser.CreateFromJSONDir("lights/dirLight");



        Shader animationShader("../../res/shaders/animationsShader.vert", "../../res/shaders/animationsShader.frag");
        HudAnimation animation1(animationShader);
        animation = animation1;
        animation.initAnimation();

        Shader UIShader("include/User/spriteShader.vert","include/User/spriteShader.frag");

        Constant constant1(animationShader);
        constant = constant1;
        constant.initConstant();

        Shader imageShader("../../res/shaders/imageShader.vert", "../../res/shaders/imageShader.frag");
        HUD hud1(imageShader);
        hud = hud1;
        hud.initImage("res/textures/tlo.png");

        for(int i = 0; i< 3; i++){
            UITips[i] = SpriteRenderer(imageShader);
        }
        UITips[0].initRenderData("res/UI/new/jumpHint.png");

//        spdlog::info("menu1 before constructor");
        Menu menu1(imageShader);
//        spdlog::info("menu1 after constructor");
        menu = menu1;
//        spdlog::info("before init1");
        menu.initMenu("../../res/UI/menu_screen.png");
//        spdlog::info("after init1");

        spdlog::info("menu2");
        Menu menu2(imageShader,true);
        menuPause = menu2;
        menuPause.initMenu("../../res/UI/pause_screen1.png");

        spdlog::info("before finished loading");
        Engine::finishedLoading();
        spdlog::info("after");

        spdlog::info("ImGui");
        ImGui();
        spdlog::info("ResolveCollisions");
        Engine::resolveCollisions();
        spdlog::info("loop");

        while (!glfwWindowShouldClose(Engine::getWindow())) {
            Update();
        }
    }

    void Update() {
        Engine::LoopStart();

        if(menu.isVisible1()){
            inputSystem.update();
            menu.processInput(&inputSystem);
            menu.drawMenu(glm::vec3(0,0,1.f),glm::vec2(800.f,600.f),0.f);

        }
        else if(menuPause.isVisible1()){
            inputSystem.update();
            menuPause.processInput(&inputSystem);
            menuPause.drawMenu(glm::vec3(0,0,1.f),glm::vec2(800.f,700.f),0.f);
        }
        else {
            AudioManager::GetInstance()->Update();
            AudioManager::GetInstance()->PlaySound(Audio::TRAIN_AMBIENT);

            ImGui();
            inputSystem.update();
            processInput();
            playerJumper.UpdatePlayer(&inputSystem, movementSpeed);
            playerGrabber.UpdatePlayer(&inputSystem, movementSpeed);


            Engine::moveObjects();

            shadows.renderShadows(*Engine::camera);
            Engine::camera->Update(Engine::deltaTime);
            glm::mat4 projection = glm::perspective(glm::radians(Engine::camera->Zoom),
                                                    (float) Engine::SCR_WIDTH / (float) Engine::SCR_HEIGHT, 0.1f,
                                                    100.0f);
            glm::mat4 view = Engine::camera->GetViewMatrix();


            background.Move(bgSpeed * Engine::deltaTime);
            if (slowdown && bgSpeed <= 0) {
                bgSpeed += 0.05;
//                spdlog::info("{}", bgSpeed);
            }
            if (slowdown && bgSpeed >= 0.0) {
                spdlog::info("ENDCREEN");
            }

            UITips[LevelManager::getInstance().currentLevel].DrawSprite(glm::vec3(550.f, 1.f, 1.f),
                                                                            glm::vec2(250.f, 200.f),0);

            Engine::renderHitboxes(projection * view);

            Engine::resolveCollisions();
        }

        Engine::LoopEnd();
    }

    void onWin() {
        if(LevelManager::getInstance().nextLevel())
        {

            playerGrabber.DropBattery();
            playerJumper.DropBattery();
            playerGrabber.levelId = LevelManager::getInstance().currentLevel;
            playerJumper.levelId = LevelManager::getInstance().currentLevel;
            grabber.levelId = LevelManager::getInstance().currentLevel;

            playerGrabber._transform._position = LevelManager::getInstance().getCurrentLevel()->playerGrabberStartingPos; //+ LevelManager::getInstance().getCurrentLevel()->_transform._position;
            playerJumper._transform._position = LevelManager::getInstance().getCurrentLevel()->playerJumperStartingPos; //+ LevelManager::getInstance().getCurrentLevel()->_transform._position;

            Engine::camera->MoveToTarget( LevelManager::getInstance().getCurrentLevel()->cameraOffset);
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
            Engine::camera->MoveToTarget( glm::vec3(90, 10.6, 17.3));
            slowdown = true;
        }
        if(LevelManager::getInstance().currentLevel==1){
            UITips[LevelManager::getInstance().currentLevel].initRenderData("res/UI/new/grabHint.png");
        }
        if(LevelManager::getInstance().currentLevel==2){
            UITips[LevelManager::getInstance().currentLevel].initRenderData("res/UI/new/ability.png");
        }
        if(LevelManager::getInstance().currentLevel>=3) {
            for (int i = 0; i < 3; i++) {
                UITips[i].isVisible = false;

            }
        }
    }

    void ResetLevel() {
        playerGrabber.DropBattery();
        playerJumper.DropBattery();
        playerGrabber._transform._position = LevelManager::getInstance().getCurrentLevel()->playerGrabberStartingPos; //+ LevelManager::getInstance().getCurrentLevel()->_transform._position;
        playerJumper._transform._position = LevelManager::getInstance().getCurrentLevel()->playerJumperStartingPos; //+ LevelManager::getInstance().getCurrentLevel()->_transform._position;
        LevelManager::getInstance().getCurrentLevel()->ResetPositions();
    }


    void ImGui() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            Engine::renderImgui();
            Engine::ImGui();
        }

        ImGui::Begin("DevTools");
        ImGui::SetWindowSize(ImVec2(250, 100));
        if (ImGui::Button("Next Level"))
            onWin();
        if (ImGui::Button("Reset Level"))
            ResetLevel();
        ImGui::End();

        ImGui::Render();
    }


    void processInput() {
            if (inputSystem.GetKeyDown(GLFW_KEY_SPACE) || inputSystem.GetGamepadButtonDown(1, GLFW_GAMEPAD_BUTTON_A)) {
                    playerJumper.Jump();
            }
            if (inputSystem.GetKeyDown(GLFW_KEY_KP_1) || inputSystem.GetGamepadButtonDown(0, GLFW_GAMEPAD_BUTTON_A)) {
                playerGrabber.Jump();

            }
            if(inputSystem.GetKeyDown(GLFW_KEY_O) || inputSystem.GetGamepadButtonDown(0, GLFW_GAMEPAD_BUTTON_START)){
                menuPause.isVisible = true;
            }
        }

};
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

namespace Game {
    void processInput();

    float imgMOv = 0;

    void ImGui();

    void renderScene(Shader shader, Camera camera);

    Shader simpleDepthShader;
    Shader debugDepthQuad;
    Shader spriteShader;


    SpriteRenderer jumpUI;

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


    //animation testing

    Hitbox p1Hitbox("hitboxes/hitbox_0");
    Hitbox p2Hitbox("hitboxes/hitbox_1");

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

    SpriteRenderer  UITips[3];
//    char Paths[7][255];




    DirectionalLight dirLight;
    SpotLight spotLight;
    PointLight pointLight;

    InputSystem inputSystem;

    std::string texToDisplay = "";

    Background background;



    void Start() {
        Engine::Init();
        spdlog::info("init engine");

        Engine::camera->ShowImgui();

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

//        playerGrabber.loadAnimation("res/models/Players/Cr4nk/crank_jumping_final.dae");
//        playerGrabber.loadAnimation("res/models/Players/Cr4nk/crank_movement_final.dae");
        playerJumper.loadAnimation("res/models/Players/Mich3l/michel_breathing_and_looking_around.dae");
        playerGrabber.loadAnimation("res/models/Players/Cr4nk/REST_CRANK_WALKING.dae");
//        Animation temp(daePath, &*_model);


//        hud.initAnimation();
//        hud.initImage("res/textures/tlo.png");
        spdlog::info("init text");
        hud2.initText("res/fonts/Arialn.ttf");


        p2Hitbox.draw = false;
        p1Hitbox.draw = false;
        p1Hitbox.tag = "player";
        p2Hitbox.tag = "player";

        //player1.loadFromJSON(Engine::parser.CreateFromJSONMovingObject("objects/movingObj_0"));
        //player2.loadFromJSON(Engine::parser.CreateFromJSONMovingObject("objects/movingObj_1"));
        //player1.tag = "player";
        //player2.tag = "player";



        //*Engine::camera = Engine::parser.CreateFromJSONCam("*Engine::camera");
        Engine::camera->ShowImgui();;

        //p1Hitbox.Create(&playerJumper);
        //p2Hitbox.Create(&playerGrabber);

        //p1Hitbox.calculateBoundingBox(player1._model);
        //p2Hitbox.calculateBoundingBox(player2._model);

        playerGrabber.grabber = &grabber;
        grabber.loadModel("res/models/Players/Cr4nk/RIGHT_HAND_CRANK_HOOKING.dae");
        grabberHitbox.draw = false;
        grabberHitbox.isTrigger = true;
//        grabber.loadAnimation("res/models/Players/Cr4nk/RIGHT_HAND_CRANK_WALKING.dae");
//        grabber.LoadAnimations();

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

        shader.setMat4("projectionView", Engine::camera->getViewProjection());

        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", model);

        dirLight = Engine::parser.CreateFromJSONDir("lights/dirLight");
//        spotLight = Engine::parser.CreateFromJSONSpot("lights/spotLight");
//        pointLight = Engine::parser.CreateFromJSONPoint("lights/pointLight");

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

        Shader UIShader("include/User/spriteShader.vert","include/User/spriteShader.frag");






        Constant constant1(animationShader);
        constant = constant1;
        constant.initConstant();

        Shader imageShader("../../res/shaders/imageShader.vert", "../../res/shaders/imageShader.frag");
        HUD hud1(imageShader);
        hud = hud1;
        hud.initImage("res/textures/tlo.png");

//        SpriteRenderer jumpingUI(imageShader);
//        jumpUI = jumpingUI;
////        jumpUI.initRenderData("res/UI/jumping_hint.png");

        for(int i = 0; i< 3; i++){
            UITips[i] = SpriteRenderer(imageShader);
        }
        UITips[0].initRenderData("res/UI/jumping_hint.png");


//        Model ourModel("include/Animations2/testing/first_character.dae");
//        Animation danceAnimation("include/Animations2/testing/first_character.dae",
//                                 &ourModel);
//        Animator animator(&danceAnimation);


        Engine::finishedLoading();
      //  Engine::logTextures();
        while (!glfwWindowShouldClose(Engine::getWindow())) {
            Update();
        }
    }

    void Update() {

        Engine::LoopStart();

//        jumpUI.DrawSprite("res/UI/A.png",glm::vec3(grabber.playerPos.x,grabber.playerPos.y,17.2), glm::vec2(10.f,10.f), 0, glm::vec3(1.f,1.f,1.f));
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


        //player1.Move();

        Engine::moveObjects();

        //Engine::renderLights(lightShader, *Engine::camera);
        shadows.renderShadows(*Engine::camera);
        Engine::camera->Update(Engine::deltaTime);
        //glm::mat4 projection = glm::mat4(1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(Engine::camera->Zoom), (float)Engine::SCR_WIDTH / (float)Engine::SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = Engine::camera->GetViewMatrix();

//
//        shader.use();
//        shader.setMat4("projectionView", projection * view);
        background.Move(-50*Engine::deltaTime);
//        hud.renderImage(imgMOv);
//        jumpUI.DrawSprite(glm::vec3(300.f, 350.f, 1.f), glm::vec2(300.f,250.f), 0);
        UITips[LevelManager::getInstance().currentLevel].DrawSprite(glm::vec3(300.f, 350.f, 1.f), glm::vec2(250.f,250.f), 0);

        Engine::renderHitboxes(projection * view);

//       *Engine::camera.followObject(player1);
        Engine::resolveCollisions();


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
        }
        if(LevelManager::getInstance().currentLevel==1){
            UITips[LevelManager::getInstance().currentLevel].initRenderData("res/UI/walking_with_chest.png");
        }
        if(LevelManager::getInstance().currentLevel==2){
            UITips[LevelManager::getInstance().currentLevel].initRenderData("res/UI/skills.png");
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
    }


    void ImGui() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            Engine::renderImgui();
            Engine::ImGui();
            dirLight.ShowImgui();
//            pointLight.ShowImgui();
//            spotLight.ShowImgui();
            Engine::camera->ShowImgui();
            shadows.ShowImgui();
            playerGrabber.ShowImgui();
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
//            jumpUI.isVisable = false;
            playerJumper.Jump();
            AudioManager::GetInstance()->PlaySound(Audio::MICHEL_JUMP);
        }
        if (inputSystem.GetKeyDown(GLFW_KEY_KP_1) || inputSystem.GetGamepadButtonDown(0, GLFW_GAMEPAD_BUTTON_A)) {
//            jumpUI.isVisable = false;
            playerGrabber.Jump();
            AudioManager::GetInstance()->PlaySound(Audio::CRANK_JUMP);
        }
    }

};
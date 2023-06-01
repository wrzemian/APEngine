#include <iostream>
#include "../include/Game.h"
#include "../include/Engine.h"

#include "../include/Engine.h"
#include "../include/User/HUD.h"
#include "../include/AssetManager.h"
#include "../include/Animations2/Animator.h"

void Game::processInput() {
    if (inputSystem.GetKeyDown(GLFW_KEY_SPACE)) {
        playerJumper.Jump(); // TODO: move to player class
    }
    if (inputSystem.GetKeyDown(GLFW_KEY_KP_1)) {
        playerGrabber.Jump(); // TODO: move to player class
    }
}


void Game::Start() {
    std::cout << Engine::getInstance().Init() << "\n";

    shadows.initShaders();
    Shader tempLightShader("../../res/shaders/shader.vert", "../../res/shaders/shader.frag");
    lightShader = tempLightShader;
    lightShader.use();

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


    p2Hitbox.draw = false;
    p1Hitbox.draw = false;
    antBigHitbox.draw = false;


    //player1.loadFromJSON(Engine::parser.CreateFromJSONMovingObject("objects/movingObj_0"));
    //player2.loadFromJSON(Engine::parser.CreateFromJSONMovingObject("objects/movingObj_1"));
    //player1.tag = "player";
    //player2.tag = "player";

    ant.loadFromJSON(Engine::getInstance().parser.CreateFromJSONMovingObject("objects/movingObj_2"));
    ant.tag = "ant";


    //camera = Engine::parser.CreateFromJSONCam("camera");
    camera.ShowImgui();;

    //p1Hitbox.Create(&playerJumper);
    //p2Hitbox.Create(&playerGrabber);

    //p1Hitbox.calculateFromModel(player1._model);
    //p2Hitbox.calculateFromModel(player2._model);
    wagon.loadFromJSON(Engine::getInstance().parser.CreateFromJSONWalls("objects/walls"));
    wagon.logNewObjects();
    wagon.setShader(&shader);
    wagon.ShowImgui();


    //background.initBackground(5,-525.509948,262.754974,&shader);

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
    if(!wagon.batteries.empty()) {
        playerJumper.battery = wagon.batteries.at(0).get();
        playerGrabber.battery = wagon.batteries.at(0).get();
    }


    playerGrabber.grabber = &grabber;
    grabber.setShader(&shader);
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

    dirLight = Engine::getInstance().parser.CreateFromJSONDir("lights/dirLight");
    spotLight = Engine::getInstance().parser.CreateFromJSONSpot("lights/spotLight");
    pointLight = Engine::getInstance().parser.CreateFromJSONPoint("lights/pointLight");

    spdlog::info("ImGui");
    ImGui();
    spdlog::info("ResolveCollisions");
    Engine::getInstance().resolveCollisions();
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



    Engine::getInstance().logTextures();
    while (!glfwWindowShouldClose(Engine::getWindow())) {
        Update();
    }
}

void Game::Update() {

    Engine::getInstance().LoopStart();
//        std::this_thread::sleep_for(std::chrono::duration<double>(1.0 / 30.0)); // to slow down frame rate for fewer collisions detection

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
    Engine::getInstance().moveObjects();

    //Engine::renderLights(lightShader, camera);
    shadows.renderShadows(camera);

    //glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)Engine::SCR_WIDTH / (float)Engine::SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
//
//        shader.use();
//        shader.setMat4("projectionView", projection * view);

    Engine::getInstance().renderHitboxes(projection * view);

//       camera.followObject(player1);
    Engine::getInstance().resolveCollisions();

    hud2.renderText(texToDisplay, 100, 0, 2, glm::vec3(1.0f, 1.0f, 1.0f));

    Engine::getInstance().LoopEnd();

}


void Game::ImGui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
        Engine::getInstance().renderImgui();
        Engine::getInstance().ImGui();
        dirLight.ShowImgui();
        pointLight.ShowImgui();
        spotLight.ShowImgui();
        camera.ShowImgui();
        shadows.ShowImgui();
    }
    ImGui::Render();
}

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
    float movImage = 0;
    HUD hud;
    HUD hud2;
    Shader shader;
    Animation animation;
    Constant constant;
//    Image image;


    MovingObject player1;
    MovingObject player2;
    Ant ant;

    Camera camera;

    Hitbox p1Hitbox("hitboxes/hitbox_0");
    Hitbox p2Hitbox("hitboxes/hitbox_1");
    SimpleHitbox antHitbox("hitboxes/hitbox_2");
    SimpleHitbox antBigHitbox("hitboxes/hitbox_3");

    Hitbox platform1Hitbox("hitboxes/hitbox_platform");
    Hitbox button1Hitbox("hitboxes/hitbox_button");

    Walls wagon;


    GLfloat movementSpeed = 10.0f;


    DirectionalLight dirLight;
    SpotLight spotLight;
    PointLight pointLight;

    InputSystem inputSystem;

    Background background;

    Platform platform(glm::vec3(-5,16,7),glm::vec3(-5,13,7),1.0f);

    Button button(&platform,glm::vec3(0,13,7));

    void Start() {
        std::cout << Engine::Init() <<"\n";

        antHitbox.isRendered = true;

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
//        hud.initImage("res/textures/tlo.png");
        hud2.initText("res/fonts/Arialn.ttf");

        player1.loadFromJSON(Engine::parser.CreateFromJSONMovingObject("objects/movingObj_0"));
        player2.loadFromJSON(Engine::parser.CreateFromJSONMovingObject("objects/movingObj_1"));
        player1.tag = "player";
        player2.tag = "player";

        ant.loadFromJSON(Engine::parser.CreateFromJSONMovingObject("objects/movingObj_2"));
        ant.tag = "ant";

        camera = Engine::parser.CreateFromJSONCam("camera");


        //p1Hitbox.calculateFromModel(player1._model);
        //p2Hitbox.calculateFromModel(player2._model);

        wagon.loadFromJSON(Engine::parser.CreateFromJSONWalls("objects/walls"));
        wagon.setShader(&shader);
//        wagon.loadModel("../../res/models/1level/1level.obj");
//        wagon.calculateHitboxes();
//        wagon.logHitboxes();
//        for(auto m: wagon._model.meshes) {
//            spdlog::info("loaded mesh with {} vertices", m.vertices.size());
//        }

        // build and compile our shader program
        // ------------------------------------
        Shader temp("../../res/shaders/shader.vert", "../../res/shaders/shader.frag");
        shader = temp;
        shader.use();

        player1.setShader(&shader);
        player2.setShader(&shader);
        ant.setShader(&shader);
        //wagon.setShader(&shader);

        platform.loadModel("../../res/models/Assets/chest1/box1.obj");
        platform.setShader(&shader);
        platform1Hitbox.Create(&platform);


        button.setShader(&shader);
        button.loadModel("../../res/models/Assets/chest1/box1.obj");
        button1Hitbox.Create(&button);
        button1Hitbox.isTrigger = true;
        //background.initBackground(5,-525.509948,262.754974,&shader);

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



        while (!glfwWindowShouldClose(Engine::getWindow())) {
            Update();
        }
    }

    void Update() {

        Engine::LoopStart();
        ImGui();
        imgMOv -= 0.1;
        inputSystem.update();
        processInput();
        movImage -= 0.1;

        float time = static_cast<float>(glfwGetTime());
        animation.renderAnimation(time,2,520,1);
        constant.renderConstant(2,550,1);
        hud.renderImage(imgMOv);

        //player1.Move();
        Engine::moveObjects();
        Engine::drawObjects(camera);


        //player1.Draw();
        //wagon.Draw();

        //box.Draw(shader);

        ////REQUIRED FOR LIGHT
        //should be property of object
        shader.setInt("material.diffuse", 0);
        shader.setInt("material.specular", 1);
        shader.setFloat("material.shininess", 32.0f);
        //from camera
        shader.setVec3("viewPos", camera.Position);

        Engine::renderLights(shader);

        //background.Move(-Engine::deltaTime*40);
        platform.UpdatePosition(Engine::deltaTime);

        //camera

        //glm::mat4 projection = glm::mat4(1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)Engine::SCR_WIDTH / (float)Engine::SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        shader.use();
        shader.setMat4("projectionView", projection * view);

        Engine::renderHitboxes(projection * view);

//       camera.followObject(player1);
        button.Update(Engine::deltaTime);
        Engine::resolveCollisions();;

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

        }
        ImGui::Render();
    }


    void processInput()
    {
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
        }
        if (inputSystem.GetKeyDown(GLFW_KEY_SPACE)) {
            player1.AddVelocity(glm::vec3(0.0f, 5.0f, 0.0f));
        }

        if (inputSystem.GetKey(GLFW_KEY_UP)) {
            player2._velocity.z += -movementSpeed;
        }
        else if (inputSystem.GetKey(GLFW_KEY_DOWN)) {
            player2._velocity.z += movementSpeed;
        }
        else {
            player2._velocity.z += 0;
        }
        if (inputSystem.GetKey(GLFW_KEY_LEFT)) {
            player2._velocity.x += -movementSpeed;
        }
        else if (inputSystem.GetKey(GLFW_KEY_RIGHT)) {
            player2._velocity.x = movementSpeed;
        }
        else {
            player2._velocity.x = 0;
        }
        if (inputSystem.GetKeyDown(GLFW_KEY_KP_1)) {
            player2.AddVelocity(glm::vec3(0.0f, 5.0f, 0.0f));
        }

//        player1.SetVelocity(glm::vec3(inputSystem.getJoystickAxis(0, GLFWD_GAMEPAD_AXIS_LEFT_X), player1._velocity.y, inputSystem.getJoystickAxis(0, GLFW_GAMEPAD_AXIS_LEFT_Y)));
//        player2.SetVelocity(glm::vec3(inputSystem.getJoystickAxis(1, GLFW_GAMEPAD_AXIS_LEFT_X),player2._velocity.y,inputSystem.getJoystickAxis(1, GLFW_GAMEPAD_AXIS_LEFT_Y)));
    }

};
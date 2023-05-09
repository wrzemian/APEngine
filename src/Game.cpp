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

//temporary
#include "../include/Rock.h"
#include "../include/Cactus.h"
#include "../include/BackgroundTile.h"
#include "../include/Background.h"

#include "spdlog/spdlog.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "al.h"
#include "alc.h"


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

    Camera camera(glm::vec3(0.f, 35.0f, 40.0f));

    Hitbox p1Hitbox;
    Hitbox p2Hitbox;

    Walls wagon;


    GLfloat movementSpeed = 10.0f;


    DirectionalLight dirLight;
    SpotLight spotLight;
    PointLight pointLight;

    InputSystem inputSystem;

    //Rock rock;
    //Cactus cactus;
    //BackgroundTile backgroundTile;
    Background background;

    void Start() {
        std::cout << Engine::Init() <<"\n";


//        camera.Position = glm::vec3(21, 5, 11);
//        camera.Front = glm::vec3(-0.6, -0.02f, -0.02f);
//        camera.Zoom = 46;


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

        //rock.initRock();
       // rock.randomizeRotation();
        //rock.randomizeSize(1,3);
        //cactus.initCactus();
        //cactus.randomizeSize(1,3);

//        hud.initAnimation();
//        hud.initImage("res/textures/tlo.png");
        hud2.initText("res/fonts/Arialn.ttf");

        player1.loadFromJSON(Engine::parser.CreateFromJSONMovingObject("movingObj_0"));
        player2.loadFromJSON(Engine::parser.CreateFromJSONMovingObject("movingObj_1"));


        p1Hitbox.Create(&player1);
        p2Hitbox.Create(&player2);

        p1Hitbox.calculateFromMesh(player1._model.meshes[0]);
        p2Hitbox.calculateFromMesh(player2._model.meshes[0]);

        wagon.loadFromJSON(Engine::parser.CreateFromJSONWalls("walls"));
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
        //wagon.setShader(&shader);

        //rock.setShader(&shader);
        //cactus.setShader(&shader);
       /* backgroundTile.setShader(&shader);
        backgroundTile.initBackgroundTile();
        backgroundTile.SetBoundaries(100,0,-1,-100);
        backgroundTile.SetAmountToGenerate(5,1,5,1);
        backgroundTile.GenerateRandomObjects();*/
        background.initBackground(2,-1500,1250,&shader);

        shader.setMat4("projectionView", camera.getViewProjection());

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


        //backgroundTile.Move(-Engine::deltaTime*10);
        background.Move(-Engine::deltaTime*40);


        //camera

        //glm::mat4 projection = glm::mat4(1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)Engine::SCR_WIDTH / (float)Engine::SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        shader.use();
        shader.setMat4("projectionView", projection * view);

        Engine::renderHitboxes(projection * view);

//        camera.followObject(player1);

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

        }
        ImGui::Render();
    }


    void processInput()
    {
        if (inputSystem.GetKey(GLFW_KEY_W))
            player1._velocity.z = -movementSpeed;
        else if (inputSystem.GetKey(GLFW_KEY_S))
            player1._velocity.z = movementSpeed;
        else
            player1._velocity.z = 0;


        if (inputSystem.GetKey(GLFW_KEY_A))
            player1._velocity.x = -movementSpeed;
        else if (inputSystem.GetKey(GLFW_KEY_D))
            player1._velocity.x = movementSpeed;
        else
            player1._velocity.x = 0;

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

//        player1.SetVelocity(glm::vec3(inputSystem.getJoystickAxis(0, GLFWD_GAMEPAD_AXIS_LEFT_X), player1._velocity.y, inputSystem.getJoystickAxis(0, GLFW_GAMEPAD_AXIS_LEFT_Y)));
//        player2.SetVelocity(glm::vec3(inputSystem.getJoystickAxis(1, GLFW_GAMEPAD_AXIS_LEFT_X),player2._velocity.y,inputSystem.getJoystickAxis(1, GLFW_GAMEPAD_AXIS_LEFT_Y)));
    }

};
#pragma once


#include "Shader.h"
#include "Camera.h"
#include "User/HUD.h"
#include "User/HudAnimation.h"
#include "lights/Shadows.h"
#include "User/Constant.h"
#include "Objects/PlayerJumper.h"
#include "Objects/PlayerGrabber.h"
#include "Objects/Ant.h"
#include "Objects/Hitbox.h"
#include "Objects/SimpleHitbox.h"
#include "Background/Background.h"

class Game {
public:
    static Game& getInstance() {
        static Game instance; // Guaranteed to be destroyed, instantiated on first use
        return instance;
    }

    Game(Game const&) = delete;  // Disallow copy constructor
    void operator=(Game const&) = delete;  // Disallow copy assignment

    void Start();
    void Update();
    void processInput();
    void ImGui();
    void renderScene(Shader shader, const Camera &camera);

private:
    Game(); // Disallow instantiating an object of this class outside the class

public:
    float imgMOv;
    Shader simpleDepthShader;
    Shader debugDepthQuad;
    float movImage;
    HUD hud;
    HUD hud2;
    Shader shader;
    HudAnimation animation;
    Shadows shadows;
    Shader lightShader;
    Constant constant;
    PlayerJumper playerJumper;
    PlayerGrabber playerGrabber;
    Ant ant;
    Camera camera;
    Hitbox p1Hitbox;
    Hitbox p2Hitbox;
    Hitbox antHitbox;
    SimpleHitbox antBigHitbox;
    Grabber grabber;
    Hitbox grabberHitbox;
    Level wagon;
    GLfloat movementSpeed;
    DirectionalLight dirLight;
    SpotLight spotLight;
    PointLight pointLight;
    InputSystem inputSystem;
    std::string texToDisplay;
    Background background;
};

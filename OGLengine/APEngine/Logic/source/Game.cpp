#include "Game.h"
#include "Triangle.h"
#include "GameObject/Component.h"
#include "GameObject/Renderer.h"
#include "HID.h"
#include <glm.hpp>
#include "Camera.h"

Triangle triangle;

HID hid;

Camera camera(glm::vec3(0.0f, 3.0f, 0.0f));

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
//
//float lastX = 800 / 2.0f;
//float lastY = 600 / 2.0f;
//bool firstMouse = true;
//bool useMouse = true;

void Game::setup()
{
    //Renderer rnd;

    //rnd.Update();

    window.init();
    
    triangle.init();

    
}

bool Game::isOn()
{
    return window.isOpen();
}

void Game::Update()
{
    window.processInput();
    window.clear();

    //logic

    //draw objects
    
    triangle.draw(camera);


    
    window.swap();
    window.pollEvents();
}



void Game::close()
{
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    window.terminate();
}

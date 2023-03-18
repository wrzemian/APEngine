#include "Game.h"
#include "Triangle.h"
#include "GameObject/Component.h"
#include "GameObject/Renderer.h"
#include "HID.h"

Triangle triangle;

HID hid;

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
    
    triangle.draw();
    
    window.swap();
    window.pollEvents();
}

void Game::close()
{
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    window.terminate();
}

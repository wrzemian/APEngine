#include "Game.h"

void Game::setup()
{
    window.init();
}

bool Game::isOn()
{
    return window.isOpen();
}

void Game::loop()
{
    window.processInput();
    window.clear();
    
    //logic

    //draw objects
    
    window.swap();
    window.pollEvents();
}

void Game::close()
{
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    window.terminate();
}

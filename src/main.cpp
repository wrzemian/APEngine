#include "../include/Game.h"
#include <iostream>

int main() {
    Engine::Init();
    Game::getInstance().Start();
}

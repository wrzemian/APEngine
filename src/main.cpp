#include "../include/Game.h"
#include "../include/Engine.h"
#include "../include/Audio/AudioManager.h"

#include <iostream>

int main() {
    Game::Start();
    AudioManager::GetInstance()->Free();
}
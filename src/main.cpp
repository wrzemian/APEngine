#include "../include/Game.h"
#include "../include/Engine.h"

#include <iostream>

int main() {
    std::cout<< "main\n";

    std::cout << Engine::Init();

    std::cout<< "init\n";

    Game::Start();
    std::cout<< "start\n";

    while(true) {
        std::cout<< "1";

        Engine::LoopStart();
        std::cout<< "2";

        Game::Update();
        std::cout<< "3";

        Engine::LoopEnd();
        std::cout<< "4";
    }


    Game::Start();
    /*while (true) {
//        Engine::LoopStart();

        Game::Update();

//        Engine::LoopEnd();
    }*/

//    std::cout << "Hello \n";
}
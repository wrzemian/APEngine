#include <iostream>
#include "Window.h"
#include "Game.h"


int main()
{
    Game game;
    game.setup();

    while (game.isOn()) {
        game.loop();
    }
    game.close();

    return 0;
}




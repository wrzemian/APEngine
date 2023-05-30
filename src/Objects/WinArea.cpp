//
// Created by Karol on 30.05.2023.
//

#include "../../include/Objects/WinArea.h"

void WinArea::onCollision(Object3D *other) {
    if(other->tag == "player")
    {
        if (std::find(players.begin(), players.end(), other) == players.end()) {
            players.push_back(other);
        }
        if(players.size() > 1)
        {
            if(!won)
            {
                won = true;
                Win();
            }
        }
    }
}

void WinArea::onCollisionExit(Object3D *other) {
    if(other->tag == "player" || other->tag == "box")
    {
        players.erase(std::remove(players.begin(), players.end(), other), players.end());
    }
}

void WinArea::Win() {
    std::string winText = "You win!";
    *text = winText;
    std::cout<< "You win!"<<std::endl;
}

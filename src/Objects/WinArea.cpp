//
// Created by Karol on 30.05.2023.
//

#include "../../include/Objects/WinArea.h"

void WinArea::onCollision(Object3D *other) {
    if(other->tag == "player")
    {
        if(other->type == "jumper" && !playerJumperInRange)
        {
            playerJumperInRange = true;
            if(playerJumperLightBulb != nullptr)
            {
                playerJumperLightBulb->ChangeColor(activeLightColor);
            }
        }
        if(other->type == "grabber" && !playerGrabberInRange)
        {
            playerGrabberInRange = true;
            if(playerGrabberLightBulb != nullptr)
            {
                playerGrabberLightBulb->ChangeColor(activeLightColor);
            }
        }

        if(canWin && playerGrabberInRange &&  playerJumperInRange)
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
    if(other->tag == "player") {
        if (other->type == "jumper" && playerJumperInRange) {
            playerJumperInRange = false;
            if(playerJumperLightBulb != nullptr)
            {
                playerJumperLightBulb->ChangeColor(inactiveLightColor);
            }
        }
        if (other->type == "grabber" && playerGrabberInRange) {
            playerGrabberInRange = false;
            if(playerGrabberLightBulb != nullptr)
            {
                playerGrabberLightBulb->ChangeColor(inactiveLightColor);
            }
        }
    }
}

void WinArea::Win() {
    Game::onWin();
}

WinArea::WinArea() {
    IGui::setWindowName("Win area");
}

void WinArea::OnActivate() {
    canWin = true;
    if(activationLightBulb != nullptr)
    {
        activationLightBulb->ChangeColor(powerActiveLightColor);
    }
}

void WinArea::OnDeactivate() {
    canWin = false;
    if(activationLightBulb != nullptr)
    {
        activationLightBulb->ChangeColor(powerInactiveLightColor);
    }
}

//
// Created by Karol on 30.05.2023.
//

#ifndef APENGINE_WINAREA_H
#define APENGINE_WINAREA_H

#include "Object3D.h"
#include "../include/Game.h"
#include "LightBulb.h"

class WinArea  : public Object3D {
private:
    bool won = false;
    bool playerGrabberInRange = false;
    bool playerJumperInRange = false;

    glm::vec3 activeLightColor = glm::vec3(0.01f,1.0f,0.0f);
    glm::vec3 inactiveLightColor = glm::vec3(1.0f,0.0f,0.0f);
    glm::vec3 powerActiveLightColor = glm::vec3(1.0f,1.0f,0.0f);
    glm::vec3 powerInactiveLightColor = glm::vec3(0.8f,0.8f,0.0f);

    bool canWin = true; //by default true and has to be set false if there is a button connected to winArea
public:
    WinArea();
    void onCollision(Object3D *other) override;
    void onCollisionExit(Object3D *other) override;
    void Win();

    void OnActivate();
    void OnDeactivate();

    void SetLights();
    std::shared_ptr<LightBulb> playerGrabberLightBulb;
    std::shared_ptr<LightBulb> playerJumperLightBulb;
    std::shared_ptr<LightBulb> activationLightBulb;

};


#endif //APENGINE_WINAREA_H

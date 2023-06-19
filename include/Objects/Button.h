//
// Created by Karol on 23.05.2023.
//

#ifndef APENGINE_BUTTON_H
#define APENGINE_BUTTON_H

#include "Object3D.h"
#include "Platform.h"
#include "WinArea.h"

class Button : public Object3D {
private:
    using super = Object3D;
    std::vector<Object3D*> objectsWithContact;
    bool isPushed = false;
    float t = 1.0f;
    float timeToReset = 1.0f;

public:
    Button(Platform* p, glm::vec3 position);

   // void Update(float dt);
    virtual void onCollision(Object3D* other);
    virtual void onCollisionExit(Object3D* other);
    void addPlatform(Platform* connectedPlatform);
    std::vector<Platform*> connectedPlatforms;

    std::vector<std::shared_ptr<Button>> connectedButtons; // only when all buttons connected are inactive then the platform will be deactivated
    void addConnectedButton(std::shared_ptr<Button> buttonToConnect);

    std::vector<std::shared_ptr<Button>> conditionalButtons; // only when all conditional buttons are pressed then the platform will be activated
    void addConditionalButton(std::shared_ptr<Button> buttonToAdd);

    bool isWinAreaConnected = false;
    std::shared_ptr<WinArea> winArea;
    void addWinArea(std::shared_ptr<WinArea> winAreaToAdd);

    void logFields();
};


#endif //APENGINE_BUTTON_H

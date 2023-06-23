//
// Created by micha on 23.06.2023.
//

#ifndef APENGINE_CABLE_H
#define APENGINE_CABLE_H

#include "Object3D.h"
//#include "Button.h"

class Cable : public Object3D {
public:
    bool isActive = false;
//    std::shared_ptr<Button> connectedButton = nullptr;
//    void connectToButton(const std::shared_ptr<Button>& button);
};


#endif //APENGINE_CABLE_H

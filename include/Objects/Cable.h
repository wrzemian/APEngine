//
// Created by micha on 23.06.2023.
//

#ifndef APENGINE_CABLE_H
#define APENGINE_CABLE_H

#include "Object3D.h"
//#include "Button.h"

class Cable : public Object3D {
private:
    bool isActive = false;
    void changeLook();

public:
    void Activate();
    void Deactivate();

};


#endif //APENGINE_CABLE_H

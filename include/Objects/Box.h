//
// Created by Karol on 28.05.2023.
//

#ifndef APENGINE_BOX_H
#define APENGINE_BOX_H

#include "MovingObject.h"

class Box : public MovingObject {
public:
    Box();
    void onCollisionY(Object3D *other) override;
};


#endif //APENGINE_BOX_H

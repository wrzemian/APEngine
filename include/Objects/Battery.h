//
// Created by Karol on 23.05.2023.
//

#ifndef APENGINE_BATTERY_H
#define APENGINE_BATTERY_H

#include "MovingObject.h"

class Battery : public MovingObject {
public:
    void onCollision(std::shared_ptr<Object3D> other) override;
};


#endif //APENGINE_BATTERY_H

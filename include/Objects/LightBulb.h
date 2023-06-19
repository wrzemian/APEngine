//
// Created by Karol on 18.06.2023.
//

#ifndef APENGINE_LIGHTBULB_H
#define APENGINE_LIGHTBULB_H
#include "Object3D.h"

class LightBulb : public Object3D {

public:
    LightBulb();
    void ChangeColor(glm::vec3 color);
};


#endif //APENGINE_LIGHTBULB_H

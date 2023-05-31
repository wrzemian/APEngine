//
// Created by Karol on 30.05.2023.
//

#ifndef APENGINE_WINAREA_H
#define APENGINE_WINAREA_H

#include "Object3D.h"

class WinArea  : public Object3D {
private:
    std::vector<Object3D*> players;
    bool won = false;
public:
    WinArea();

    std::string * text;
    void onCollision(Object3D *other) override;
    void onCollisionExit(Object3D *other) override;
    void Win();
};


#endif //APENGINE_WINAREA_H

//
// Created by wrzos on 22.05.2023.
//

#ifndef APENGINE_ANT_H
#define APENGINE_ANT_H


#include "MovingObject.h"

class Ant : public MovingObject, public std::enable_shared_from_this<Ant> {
private:
    using super = MovingObject;

public:
    Ant();

    virtual ~Ant();

    float timer;
    float movingTime;
    float minSpeed;
    float maxSpeed;
    float refreshTime;
    float minMoveTime;
    float maxMoveTime;
    bool isMoving;

    void ImGui() override;
    void Move(float deltaTime) override;

    void Escape(Object3D* other);
    void onCollisionX(Object3D* other);
    void onCollisionY(Object3D* other);
    void onCollisionZ(Object3D* other);


};


#endif //APENGINE_ANT_H

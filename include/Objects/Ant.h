//
// Created by wrzos on 22.05.2023.
//

#ifndef APENGINE_ANT_H
#define APENGINE_ANT_H


#include "MovingObject.h"

class Ant : public MovingObject {
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

    void Escape(std::shared_ptr<Object3D> other);
    void onCollisionX(std::shared_ptr<Object3D> other) override;
    void onCollisionY(std::shared_ptr<Object3D> other) override;
    void onCollisionZ(std::shared_ptr<Object3D> other) override;

};


#endif //APENGINE_ANT_H

//
// Created by wrzos on 23.05.2023.
//

#ifndef APENGINE_SIMPLEHITBOX_H
#define APENGINE_SIMPLEHITBOX_H


#include "Hitbox.h"

class SimpleHitbox : public Hitbox {
private:
    using super = Hitbox;
public:
    SimpleHitbox() = default;
    SimpleHitbox(std::string fileName);

    ~SimpleHitbox();

    void resolveCollision(Hitbox& other) override;

};


#endif //APENGINE_SIMPLEHITBOX_H

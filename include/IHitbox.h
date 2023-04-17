//
// Created by Misza on 17.04.2023.
//

#pragma once
#include "spdlog/spdlog.h"

class IHitbox {
public:
    IHitbox();

    std::string tag;

    virtual bool TestForIntersection(IHitbox &other) {
        spdlog::warn("hitbox default intersection");
        return false;
    };

};



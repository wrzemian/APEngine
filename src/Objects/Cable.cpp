//
// Created by micha on 23.06.2023.
//

#include "../../include/Objects/Cable.h"


void Cable::Activate() {
    if(isActive) { // stop running early if already on
        return;
    }
    isActive = true; // change state
    changeLook();
}

void Cable::Deactivate() {
    if(!isActive) { // stop running early if already off
        return;
    }
    isActive = false; // change state
    changeLook();
}

void Cable::changeLook() {
    if(isActive) {
        // TODO: add turning on emmisive
        spdlog::info("Cable {} {} turned ON", id, _path);
    } else {
        // TODO: add turning off emmisive
        spdlog::info("Cable {} {} turned OFF", id, _path);
    }
}
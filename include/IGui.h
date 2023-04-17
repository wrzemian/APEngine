//
// Created by Misza on 17.04.2023.
//

#pragma once

#include <string>
#include <spdlog/spdlog.h>

class IGui {

public:
    IGui();

    void setWindowName(std::string name) {
        windowName = name + " #" + windowName;
    }

    std::string windowName;

    virtual void ImGui() {
        spdlog::warn("default IGui");
    }

};
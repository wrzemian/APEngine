//
// Created by Misza on 17.04.2023.
//

#pragma once

#include <string>
#include <spdlog/spdlog.h>
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"

class IGui {

public:
    bool isRendered = false;

    IGui();

    virtual ~IGui();

    void setWindowName(std::string name) {
        windowName = name + " #" + windowName;
       // spdlog::info("set window name to: {}", windowName);

    }

    std::string windowName;

    virtual void ImGui() {
        spdlog::warn("default IGui");
    }

};
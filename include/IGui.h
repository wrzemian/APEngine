//
// Created by Misza on 17.04.2023.
//

#pragma once

#include <string>
#include <spdlog/spdlog.h>
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"

class IGui {

private:
    bool _showImgui = false;
    std::string _windowName;

public:
    IGui();
    virtual ~IGui();

    [[nodiscard]] bool isShownImgui() const;

    void ShowImgui();
    void HideImgui();

    [[nodiscard]] const std::string &getWindowName() const;

    void setWindowName(std::string const& name);

    virtual void ImGui();

};
//
// Created by Misza on 17.04.2023.
//

#pragma once

#include <string>
#include <spdlog/spdlog.h>
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"

class MovingObject;
class Object3D;
class Hitbox;

class IGui : public std::enable_shared_from_this<IGui>  {

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

    std::shared_ptr<Object3D> getSharedObject();

    std::shared_ptr<MovingObject> getSharedMovingObject();

    std::shared_ptr<Hitbox> getSharedHitbox();
};
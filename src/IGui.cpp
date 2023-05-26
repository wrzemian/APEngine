//
// Created by Misza on 17.04.2023.
//

#include "../include/IGui.h"
#include "../include/Engine.h"


IGui::IGui() {
    Engine::addImgui(shared_from_this());
    _windowName = std::to_string(Engine::getImguiIndex());
}

IGui::~IGui() {
    Engine::removeImgui(shared_from_this());
}

bool IGui::isShownImgui() const {
    return _showImgui;
}

void IGui::ShowImgui() {
    IGui::_showImgui = true;
}

void IGui::HideImgui() {
    IGui::_showImgui = false;
}

const std::string &IGui::getWindowName() const {
    return _windowName;
}

void IGui::setWindowName(std::string const& name) {
    _windowName = name + " #" + _windowName;
}

void IGui::ImGui() {
    spdlog::error("default IGui");
}

std::shared_ptr<Object3D> IGui::getSharedObject()  {
    return std::static_pointer_cast<Object3D>(shared_from_this());
}

std::shared_ptr<MovingObject> IGui::getSharedMovingObject()  {
    return std::static_pointer_cast<MovingObject>(shared_from_this());
}

std::shared_ptr<Hitbox> IGui::getSharedHitbox()  {
    return std::static_pointer_cast<Hitbox>(shared_from_this());
}
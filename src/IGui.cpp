//
// Created by Misza on 17.04.2023.
//

#include "../include/IGui.h"
#include "../include/Engine.h"


IGui::IGui() {
    Engine::addImgui(this);
    _windowName = std::to_string(Engine::getImguiIndex());
}

IGui::~IGui() {
    Engine::removeImgui(this);
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

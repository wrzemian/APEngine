//
// Created by Misza on 17.04.2023.
//

#include "../include/IGui.h"
#include "../include/Engine.h"


IGui::IGui() {
    Engine::getInstance().addImgui(this);
    _windowName = std::to_string(Engine::getInstance().getImguiIndex());
}

IGui::~IGui() {
    Engine::getInstance().removeImgui(this);
}

bool IGui::isShownImgui() const {
    return _showImgui;
}

void IGui::ShowImgui() {
    IGui::_showImgui = true;
}

void IGui::setImgui(bool show) {
    IGui::_showImgui = show;
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

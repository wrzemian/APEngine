//
// Created by Misza on 17.04.2023.
//

#include "../include/IGui.h"
#include "../include/Engine.h"


IGui::IGui() {
    isRendered = true;
    Engine::addImgui(this);
    windowName = std::to_string(Engine::getImguiIndex());

}

IGui::~IGui() {
    Engine::removeImgui(this);

}

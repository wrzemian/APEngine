//
// Created by Karol on 02.04.2023.
//

#ifndef APENGINE_INPUTSYSTEM_H
#define APENGINE_INPUTSYSTEM_H

#include "../include/Engine.h"

class InputSystem {
public:
    // Window should be initialised once at the beginning,
    // we dont want to provide it every time we check input
    // Also, key code as int? Not very convenient for future use
    static bool GetKeyDown(GLFWwindow* window, int key);

};


#endif //APENGINE_INPUTSYSTEM_H

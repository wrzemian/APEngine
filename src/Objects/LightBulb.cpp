//
// Created by Karol on 18.06.2023.
//

#include "../../include/Objects/LightBulb.h"

LightBulb::LightBulb() {

}

void LightBulb::ChangeColor(glm::vec3 color) {
    std::cout << "light bulb changed color to : " << color.x << ", " <<  color.y << ", " << color.z << std::endl;
}


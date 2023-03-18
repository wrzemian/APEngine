#pragma once
#include <glad/glad.h>
#include <glfw3.h>

#include <string>

class HID
{
public:
	bool GetButton(std::string Key);
};
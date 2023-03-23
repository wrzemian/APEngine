#pragma once
#include <glad/glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <Camera.h>

class Window
{
private:
	// settings
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;

	GLFWwindow* _window;

public:

	int init();
	void processInput();
	bool isOpen();
	void clear();
	void swap();
	void pollEvents();
	void terminate();

	GLFWwindow* getWindow();
};


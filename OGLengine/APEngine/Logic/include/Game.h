#pragma once
#include "Window.h"

class Game
{

public:
	void setup();
	bool isOn();
	void Update();
	void close();
private:
	Window window;

};


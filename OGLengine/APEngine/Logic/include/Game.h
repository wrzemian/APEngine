#pragma once
#include "Window.h"

class Game
{

public:
	void setup();
	bool isOn();
	void loop();
	void close();
private:
	Window window;

};


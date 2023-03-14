#pragma once

#include <string>

class Component
{
public:
	Component() {};

	virtual std::string GetType() = 0;
	virtual bool IsType() = 0;
	virtual void ReceiveEvent() = 0;
	virtual void Update() = 0;
};


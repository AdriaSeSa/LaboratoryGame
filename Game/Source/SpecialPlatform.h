#pragma once
#include "MobilePlatform.h"

class FireTrap;

class SpecialPlatform :  public MobilePlatform
{
public:

	SpecialPlatform(iPoint position, std::string name, std::string tag, Application* app, int lenght = 2, iPoint moveDistance = { 0,0 }, float moveSpeed = 1.0f, int stopTime = 0);

	void Update() override;

private:

	FireTrap* fireTraps[20] = { nullptr };
};


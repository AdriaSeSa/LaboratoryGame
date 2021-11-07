#pragma once
#include "GameObject.h"
#include "Animation.h"

class FireTrap :public GameObject
{
public: 

	FireTrap(iPoint position, std::string name, std::string tag, Application* app);

	void Update() override;

	void PostUpdate() override;

	void FireOn(int flashDuration = 0, int fireDuration = 0);

private:

	int flashDuration = 0;
	int fireDuration = 0;

	bool activeteFire = false;
	Animation fireFlash;
	Animation fireOn;
};


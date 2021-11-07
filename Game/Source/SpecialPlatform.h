#pragma once
#include "MobilePlatform.h"

#define MAX_FIRENUM 20

class FireTrap;

class SpecialPlatform :  public MobilePlatform
{
public:

	SpecialPlatform(iPoint position, std::string name, std::string tag, Application* app, int lenght = 2, iPoint moveDistance = { 0,0 }, float moveSpeed = 1.0f, int stopTime = 0);

	void Reset();

	void Update() override;

	void PostUpdate() override;

	void OnCollisionEnter(PhysBody* col);

	void CleanUp() override;

private:

	FireTrap* fireTraps[MAX_FIRENUM] = { nullptr };

	int fireStep = 0;
};


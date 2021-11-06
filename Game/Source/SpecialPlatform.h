#pragma once
#include "MobilePlatform.h"
class SpecialPlatform :  public MobilePlatform
{
public:

	SpecialPlatform(iPoint position, std::string name, std::string tag, Application* app, int lenght = 2, iPoint moveDistance = { 0,0 }, float moveSpeed = 1.0f, int stopTime = 0);

	void Update() override;

	void OnCollisionEnter(PhysBody* col) override;

	bool startMove = false;
};


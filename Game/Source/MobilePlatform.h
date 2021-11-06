#pragma once
#include "GameObject.h"

class MobilePlatform : public GameObject
{
public:
	MobilePlatform(iPoint position, std::string name, std::string tag, Application* app, int lenght = 3, iPoint moveDistance = { 0,0 }, float moveSpeed = 1.0f, int stopTime = 0);

	void Update() override;

	void PostUpdate() override;

	void OnCollisionEnter(PhysBody* col) override;

	void OnCollisionExit(PhysBody* col) override;

public:
	int lenght = 2;
	float speed;
	int stopTime = 0;
	int countStopTime = stopTime;

private:

	iPoint startPos;
	iPoint endPos;

	// 0 = start, 1 = end;
	int moveState = 0;

	iPoint moveDir = {0,0};

};


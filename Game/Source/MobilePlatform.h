#pragma once
#include "Platform.h"

class MobilePlatform : public Platform
{
public:
	MobilePlatform(iPoint position, std::string name, std::string tag, Application* app, int lenght = 2, iPoint moveDistance = { 0,0 }, float moveSpeed = 1.0f, bool loop = true, int stopTime = 0);

	void Update() override;

	void OnCollisionEnter(PhysBody* col);

public:
	float speed;
	int stopTime = 0;
	int countStopTime = stopTime;
	bool startMove = false;
	bool loop = true;

protected:

	b2Vec2 startVeclocity;

protected:

	iPoint startPos;
	iPoint endPos;
	// 0 = start, 1 = end;
	int moveState = 0;

	iPoint moveDir = {0,0};
};


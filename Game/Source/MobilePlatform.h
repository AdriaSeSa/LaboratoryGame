#pragma once
#include "Platform.h"

class MobilePlatform : public Platform
{
public:
	MobilePlatform(iPoint position, std::string name, std::string tag, Application* app, int lenght = 2, iPoint moveDistance = { 0,0 }, float moveSpeed = 1.0f, bool loop = true, int stopTime = 0);

	void Update() override;

	void OnCollisionEnter(PhysBody* col);

	void InitStates(iPoint moveDistance);

	void Move();

	void Idle();

public:
	float speed = 0;
	int stopTime = 0;
	int countStopTime = stopTime;
	bool startMove = false;
	bool loop = true;

protected:

	b2Vec2 startVeclocity = {0,0};

protected:

	iPoint startPos = {0,0};
	iPoint endPos = { 0,0 };
	// 0 = start, 1 = end;
	int moveState = 0;
	int distance = 0;

	iPoint moveDir = {0,0};
};


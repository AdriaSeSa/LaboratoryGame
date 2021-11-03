#pragma once
#include "GameObject.h"

class MobilePlatform : public GameObject
{
public:
	MobilePlatform(iPoint position, std::string name, std::string tag, Application* app, int lenght = 2, iPoint moveDistance = { 0,0 }, float moveSpeed = 1.0f);

	void Update() override;

	void PostUpdate() override;

public:
	int lenght = 2;
	float speed;

	iPoint startPos;
	iPoint endPos;

	// 0 = start, 1 = move, 2 = end;
	int moveState = 0;
};


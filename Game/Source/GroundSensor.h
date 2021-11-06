#pragma once
#include "GameObject.h"
class GroundSensor : public GameObject
{
public:
	GroundSensor(iPoint pos, std::string name, std::string tag, Application* app, int width, int height);

	void OnCollisionEnter(PhysBody* col) override;

	void OnCollisionExit(PhysBody* col) override;

	void SetOffGround();

	bool isOnGround = true;
};


#pragma once
#include "GameObject.h"
class GroundSensor : public GameObject
{
public:
	GroundSensor(iPoint pos, std::string name, std::string tag, Application* app);

	void OnCollision(PhysBody* col) override;

	void SetOffGround();

	bool isOnGround = true;
};


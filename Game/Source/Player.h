#pragma once
#include "GameObject.h"
class Player : public GameObject
{
public:
	Player(iPoint pos, std::string name, std::string tag, Application* app);

	void Update() override;

	bool appliedFallForce = false;
};


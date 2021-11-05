#pragma once
#include "GameObject.h"
#include "GroundSensor.h"

class Player : public GameObject
{
public:
	Player(iPoint pos, std::string name, std::string tag, Application* app);
	~Player();
	void Update() override;

	void OnCollision(PhysBody* col) override;

	void CleanUp() override;

	bool appliedFallForce = false;
	bool isFalling;
	bool isOnGround;

	GroundSensor* groundSensor;
	iPoint groundSensorOffset = {0,8 };

	int jumpCount = 2;

	float speed = 5;
};


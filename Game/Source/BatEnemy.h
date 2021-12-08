#pragma once
#include "Enemy.h"
#include "HitboxSensor.h"

class BatEnemy : public Enemy
{
private:
	iPoint initialPos;

	HitboxSensor* detectionSensor;

	int detectionRadius = 100;

public:
	BatEnemy(iPoint pos, Player* player, std::string name, std::string tag, Application* app);

	void Update();

	void OnCollisionEnter(PhysBody* col) override;

	void OnTriggerEnter(PhysBody* col) override;

	void OnTriggerExit(PhysBody* col) override;

	void CleanUp() override;
};
#pragma once
#include "Enemy.h"
#include "HitboxSensor.h"

enum BAT_STATE
{
	BAT_IDLE,
	BAT_FLYING,
	BAT_CEILINGIN,
	BAT_CEILINGOUT,
	BAT_HIT
};

class BatEnemy : public Enemy
{
private:
	iPoint initialPos;

	HitboxSensor* detectionSensor;

	int detectionRadius = 140;

	Animation anims[5];

	Animation currentAnim;

	BAT_STATE batState;

public:
	BatEnemy(iPoint pos, Player* player, std::string name, std::string tag, Application* app);

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void OnCollisionEnter(PhysBody* col) override;

	void OnTriggerEnter(PhysBody* trigger, PhysBody* col) override;

	void OnTriggerExit(PhysBody* trigger, PhysBody* col) override;

	void CleanUp() override;

	void SetUpAnimations() override;

	void ChangeState(BAT_STATE state);
};
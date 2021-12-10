#pragma once
#include "Enemy.h"

enum CHAMELEON_STATE
{
	CHAMELEON_IDLE,
	CHAMELEON_RUN,
	CHAMELEON_ATTACK,
	CHAMELEON_HIT
};

class ChameleonEnemy :  public Enemy
{
private:
	HitboxSensor* detectionSensor;

	HitboxSensor* attack;

	Animation anims[5];

	CHAMELEON_STATE chameleonState;

	bool flip = false;

public:
	ChameleonEnemy(iPoint pos, Player* player, std::string name, std::string tag, Application* app);

	void Update() override;

	void PreUpdate() override;

	void PostUpdate() override;

	void OnCollisionEnter(PhysBody* col) override;

	void OnTriggerEnter(PhysBody* trigger, PhysBody* col) override;

	void OnTriggerExit(PhysBody* trigger, PhysBody* col) override;

	void CleanUp() override;

	void SetUpAnimations();

	void CheckFlip();

	void Attack();

	void ChangeState(CHAMELEON_STATE state);
};


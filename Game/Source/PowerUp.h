#pragma once
#include "GameObject.h"
#include "Animation.h"

class PowerUp : public GameObject
{
public: 

	PowerUp();

	PowerUp(iPoint pos, std::string name, std::string tag, Application* app);

	void PostUpdate() override;

	void OnCollisionEnter(PhysBody* col) override;

	void OnCollisionExit(PhysBody* col) override;

private:
	Animation idleAnim;

	Animation collectedAnim;

	bool collected = false;
};


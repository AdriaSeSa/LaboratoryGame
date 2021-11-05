#pragma once
#include "GameObject.h"
#include "Animation.h"

class PowerUp : public GameObject
{
public: 

	PowerUp();

	PowerUp(iPoint pos, std::string name, std::string tag, Application* app);

	void PostUpdate() override;

	void OnCollision(PhysBody* col) override;

private:
	Animation anim;
};


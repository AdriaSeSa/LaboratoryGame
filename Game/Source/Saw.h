#pragma once
#include "GameObject.h"
#include "Animation.h"

class Saw : public GameObject
{
public:
	Saw(iPoint pos, std::string name, std::string tag, Application* app);

	void PostUpdate() override;

private:
	Animation anim;
};


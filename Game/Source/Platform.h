#pragma once
#include "GameObject.h"
class Platform : public GameObject
{
public:

	Platform(iPoint position, std::string name, std::string tag, Application* app, int lenght);

	int lenght;

	bool triggerPlatform = true;

	void PostUpdate() override;

	void OnCollisionEnter(PhysBody* col) override;

	void OnCollisionExit(PhysBody* col) override;
};


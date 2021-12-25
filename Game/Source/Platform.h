#pragma once
#include "GameObject.h"
class Platform : public GameObject
{
public:

	Platform(iPoint position, std::string name, std::string tag, Application* app, int lenght);

	void PostUpdate() override;

	void OnCollisionEnter(PhysBody* col) override;

	void OnCollisionExit(PhysBody* col) override;

public:
	int lenght = 0;

	bool triggerPlatform = true;

protected:

	int offsetY = 0;
};


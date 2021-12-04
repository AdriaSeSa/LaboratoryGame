#pragma once
#include "GameObject.h"
class GroundSensor : public GameObject
{
public:
	GroundSensor(iPoint pos, std::string name, std::string tag, Application* app, int width, int height, GameObject* father);

	void OnCollisionEnter(PhysBody* col) override;

	void OnCollisionExit(PhysBody* col) override;

	void SetOffGround();

	bool isOnGround = true;

	GameObject* father = nullptr;

private:

	string colObjects[6] = { "Wall" ,"Platform","MobilePlatform","MobilePlatform_H","FireTrap" };
};


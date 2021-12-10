#pragma once
#include "GameObject.h"
class HitboxSensor : public GameObject
{
public:

	HitboxSensor(iPoint pos, int width, int height, GameObject* father, std::string name, std::string tag, Application* app);

	HitboxSensor(iPoint pos, int radius, GameObject* father, std::string name, std::string tag, Application* app);

	void Update() override;

	void OnCollisionEnter(PhysBody* col);

	void OnCollisionExit(PhysBody* col);

	std::string hits[6];

private:
	GameObject* father;

	List<PhysBody*> collisionList;
};


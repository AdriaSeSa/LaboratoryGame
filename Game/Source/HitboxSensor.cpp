#include "HitboxSensor.h"

HitboxSensor::HitboxSensor(iPoint pos, int width, int height, GameObject* father, std::string name, std::string tag, Application* app) : GameObject(name, tag, app)
{
	pBody = _app->physics->CreateRectangle(pos, width, height, this);
	pBody->body->GetFixtureList()->SetSensor(true);

	this->father = father;
}

void HitboxSensor::OnCollisionEnter(PhysBody* col)
{
  	if (col->gameObject == father) return;
	for (int i = 0; i < 6; i++)
	{
		if (col->gameObject->name == hits[i])
		{
			father->OnTriggerEnter(col);
		}
	}
	
}

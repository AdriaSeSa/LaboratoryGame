#include "HitboxSensor.h"

HitboxSensor::HitboxSensor(iPoint pos, int width, int height, GameObject* father, std::string name, std::string tag, Application* app) : GameObject(name, tag, app)
{
	pBody = _app->physics->CreateRectangle(pos, width, height, this);
	pBody->body->GetFixtureList()->SetSensor(true);
	pBody->body->SetType(b2BodyType::b2_kinematicBody);

	this->father = father;
}

HitboxSensor::HitboxSensor(iPoint pos, int radius, GameObject* father, std::string name, std::string tag, Application* app) : GameObject(name, tag, app)
{
	pBody = _app->physics->CreateCircle(pos.x, pos.y, radius, this, true);
	pBody->body->SetType(b2BodyType::b2_kinematicBody);

	this->father = father;
}

void HitboxSensor::Update()
{
	for (int i = 0; i < collisionList.count(); i++)
	{
		father->OnTriggerStay(pBody, collisionList[i]);
	}
}

void HitboxSensor::OnCollisionEnter(PhysBody* col)
{
   	if (col->gameObject == father) return;

	for (int i = 0; i < 6; i++)
	{
		if (col->gameObject->name == hits[i])
		{
 			father->OnTriggerEnter(pBody,col);
			collisionList.add(col);
		}
	}
}

void HitboxSensor::OnCollisionExit(PhysBody* col)
{
	if (col->gameObject == father) return;
	for (int i = 0; i < 6; i++)
	{
		if (col->gameObject->name == hits[i])
		{
			father->OnTriggerExit(pBody, col);
			collisionList.remove(collisionList.At(collisionList.find(col)));
		}
	}
}

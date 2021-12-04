#include "GroundSensor.h"

GroundSensor::GroundSensor(iPoint pos, std::string name, std::string tag, Application* app, int width, int height, GameObject* father) : GameObject(name, tag, app)
{
	pBody = _app->physics->CreateRectangle(pos, width, height, this);
	pBody->body->GetFixtureList()->SetSensor(true);
	this->father = father;
}

void GroundSensor::OnCollisionEnter(PhysBody* col)
{
	for (int i = 0; i < 6 ; i++)
	{
		if(col->gameObject->CompareTag(colObjects[i]))
		{
			isOnGround = true;
		}
	}
}

void GroundSensor::OnCollisionExit(PhysBody* col)
{
	for (int i = 0; i < 6; i++)
	{
		if (col->gameObject->CompareTag(colObjects[i]))
		{
			isOnGround = false;
		}
	}
}

void GroundSensor::SetOffGround()
{
	isOnGround = false;
}

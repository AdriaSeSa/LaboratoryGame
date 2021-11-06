#include "GroundSensor.h"

GroundSensor::GroundSensor(iPoint pos, std::string name, std::string tag, Application* app, int width, int height) : GameObject(name, tag, app)
{
	pBody = _app->physics->CreateRectangle(pos, width, height, this);
	pBody->body->GetFixtureList()->SetSensor(true);
}

void GroundSensor::OnCollisionEnter(PhysBody* col)
{
	if (col->gameObject->tag == "Wall" || col->gameObject->tag == "Platform" || col->gameObject->tag == "MobilePlatform")
	{
		isOnGround = true;
	}	
}

void GroundSensor::OnCollisionExit(PhysBody* col)
{
	if (col->gameObject->tag == "Wall" || col->gameObject->tag == "Platform" || col->gameObject->tag == "MobilePlatform")
	{
		isOnGround = false;
	}
}

void GroundSensor::SetOffGround()
{
	isOnGround = false;
}

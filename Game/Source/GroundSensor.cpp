#include "GroundSensor.h"

GroundSensor::GroundSensor(iPoint pos, std::string name, std::string tag, Application* app) : GameObject(name, tag, app)
{
	pBody = _app->physics->CreateRectangle(pos.x, pos.y, 8, 2, this);
	pBody->body->GetFixtureList()->SetSensor(true);
}

void GroundSensor::OnCollision(PhysBody* col)
{
	if (col->gameObject->tag == "Wall" || col->gameObject->tag == "Platform" || col->gameObject->tag == "MobilePlatform")
	{
		isOnGround = true;
	}
	
}

void GroundSensor::SetOffGround()
{
	isOnGround = false;
}

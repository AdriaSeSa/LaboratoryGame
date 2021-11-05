#include "PowerUp.h"

PowerUp::PowerUp(iPoint pos, std::string name, std::string tag, Application* app) :GameObject(name, tag, app)
{
	pBody = _app->physics->CreateRectangle(pos, 10, 10, this);
	pBody->body->SetFixedRotation(true);
	pBody->body->GetFixtureList()->SetSensor(true);
}

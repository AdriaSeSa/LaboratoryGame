#include "Spike.h"

Spike::Spike(iPoint pos, int angle, std::string name, std::string tag, Application* app) : GameObject(name, tag, app)
{
	InitRenderObjectWithXml();

	pBody = _app->physics->CreateRectangle(pos.x, pos.y, 16, 8);
	SetRotation(angle);
	pBody->body->SetType(b2BodyType::b2_staticBody);
}
#include "Spike.h"

Spike::Spike(iPoint pos, int angle, std::string name, std::string tag, Application* app) : GameObject(name, tag, app)
{
	InitRenderObjectWithXml();

	iPoint offset;

	switch (angle)
	{
	case 0:
		offset = { 0,4 };
		break;
	case 90:
		offset = { -4,0 };
		break;
	case 180:
		offset = { 0,-4 };
		break;
	case 270:
		offset = { 4,0 };
		break;
	}

	pos += offset;

	pBody = _app->physics->CreateRectangle(pos.x, pos.y, renderObjects[0].destRect.w, renderObjects[0].destRect.h, this);
	SetRotation(angle);
	pBody->body->SetType(b2BodyType::b2_staticBody);
	pBody->body->GetFixtureList()->SetSensor(true);
}

void Spike::PostUpdate()
{
	GameObject::PostUpdate();
}

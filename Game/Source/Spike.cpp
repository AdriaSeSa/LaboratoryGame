#include "Spike.h"

Spike::Spike(iPoint pos, int angle, std::string name, std::string tag, Application* app) : GameObject(name, tag, app)
{
	renderObjects[0].texture = _app->textures->Load("Assets/textures/Traps/Spikes/idle.png");

	renderObjects[0].rotation = angle;
	renderObjects[0].rotationEnabled = false;
	renderObjects[0].layer = 1;
	renderObjects[0].section = { 0,0,16,16 };

	pBody = _app->physics->CreateRectangle(pos.x, pos.y, 16, 8);
	pBody->body->SetFixedRotation(angle);
	pBody->body->SetType(b2BodyType::b2_staticBody);
}
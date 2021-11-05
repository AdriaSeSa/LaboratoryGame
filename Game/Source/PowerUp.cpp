#include "PowerUp.h"

PowerUp::PowerUp(iPoint pos, std::string name, std::string tag, Application* app) :GameObject(name, tag, app)
{
	// Init renderObject
	InitRenderObjectWithXml();

	for (int i = 0; i < 17; i++)
	{
		anim.PushBack({ i * renderObjects[0].destRect.w,0,renderObjects[0].destRect.w,renderObjects[0].destRect.h });
	}
	anim.hasIdle = false;
	anim.speed = 0.4f;

	pBody = _app->physics->CreateCircle(pos.x, pos.y, 6, this);
	pBody->body->SetType(b2_kinematicBody);
	pBody->body->SetFixedRotation(true);
	pBody->body->GetFixtureList()->SetSensor(true);
}

void PowerUp::PostUpdate()
{
	anim.Update();
	renderObjects[0].section = anim.GetCurrentFrame();

	GameObject::PostUpdate();
}

void PowerUp::OnCollision(PhysBody* col)
{
	if (col->gameObject->CompareTag("Player"))
	{
		//printf("Fruit col player");
	}
}

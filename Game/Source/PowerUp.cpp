#include "PowerUp.h"

PowerUp::PowerUp(iPoint pos, std::string name, std::string tag, Application* app) :GameObject(name, tag, app)
{
	// Init renderObject
	InitRenderObjectWithXml(name);
	InitRenderObjectWithXml("collected", 1);
	renderObjects[1].draw = false;

	//InitRenderObjectWithXml("collected", 1);

	for (int i = 0; i < 17; i++)
	{
		idleAnim.PushBack({ i * renderObjects[0].destRect.w,0,renderObjects[0].destRect.w,renderObjects[0].destRect.h });
	}
	idleAnim.hasIdle = false;
	idleAnim.speed = 0.4f;

	for (int i = 0; i < 6; i++)
	{
		collectedAnim.PushBack({ i * renderObjects[0].destRect.w,0,renderObjects[0].destRect.w,renderObjects[0].destRect.h });
	}
	collectedAnim.loop = false;
	collectedAnim.speed = 0.2f;
	collectedAnim.hasIdle = false;

	pBody = _app->physics->CreateCircle(pos.x, pos.y, 5, this);
	pBody->body->SetType(b2_kinematicBody);
	pBody->body->SetFixedRotation(true);
	pBody->body->GetFixtureList()->SetSensor(true);
}

void PowerUp::PostUpdate()
{
	if(!collected)
	{
		idleAnim.Update();
		renderObjects[0].section = idleAnim.GetCurrentFrame();
	}
	else
	{
		if (collectedAnim.HasFinished())
		{
			pendingToDelete = true;
		}
		else
		{
			collectedAnim.Update();
			renderObjects[1].section = collectedAnim.GetCurrentFrame();
		}	
	}

	GameObject::PostUpdate();
}

void PowerUp::OnCollisionEnter(PhysBody* col)
{
	if (col->gameObject->CompareTag("Player"))
	{
		if(!collected)
		{
			//printf("Fruit col player");
			renderObjects[0].draw = false;
			renderObjects[1].draw = true;
			collected = true;
		}
	}
}

void PowerUp::OnCollisionExit(PhysBody* col)
{
	//printf_s("ColFruitExit\n");
}

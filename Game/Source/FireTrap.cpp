#include "FireTrap.h"
#include "Player.h"

FireTrap::FireTrap(iPoint position, std::string name, std::string tag, Application* app)
	:GameObject(name, tag, app)
{
	startPos = position;

	// Init renderObject
	std::string texNames[2] = { "fireTrampFlash","fireTrampOn" };
	for (int i = 0; i < 2; i++)
	{
		InitRenderObjectWithXml(texNames[i], i);
	}

	// Init Anim
	for (int i = 0; i < 2; i++)
	{
		fireFlash.PushBack({ i * renderObjects[0].destRect.w,0,renderObjects[0].destRect.w,renderObjects[0].destRect.h });
	}
	fireFlash.hasIdle = false;
	fireFlash.loop = true;
	fireFlash.speed = 0.2f;

	for (int i = 0; i < 3; i++)
	{
		fireOn.PushBack({ i * renderObjects[1].destRect.w,0,renderObjects[1].destRect.w,renderObjects[1].destRect.h });
	}
	fireOn.hasIdle = false;
	fireOn.loop = true;
	fireOn.speed = 0.3f;
	renderObjects[1].draw = false;

	// Init pBody
	pBody = _app->physics->CreateRectangleSensor({ position.x, position.y }, 10, renderObjects[0].destRect.h, this);
}

void FireTrap::Reset()
{
	flashDuration = 0;
	fireDuration = 0;
	SetPosition(startPos);
	SetLinearVelocity({ 0,0 });
}

void FireTrap::Update()
{
	if(activeteFire)
	{
		if (flashDuration > 0)
		{
			flashDuration--;
		}
		else if (fireDuration > 0)
		{
			fireDuration--;
		}
		else
		{
			activeteFire = false;
			pBody->SetSensor(true);
		}
	}
}

void FireTrap::PostUpdate()
{
	if (activeteFire)
	{
		if (flashDuration > 0)
		{
			// Flashing
			fireFlash.Update();
			renderObjects[1].draw = false;
			renderObjects[0].draw = true;
			renderObjects[0].section = fireFlash.GetCurrentFrame();
		}
		else
		{
			// Fire
			fireOn.Update();
			renderObjects[0].draw = false;
			renderObjects[1].draw = true;
			renderObjects[1].section = fireOn.GetCurrentFrame();
			if (player != nullptr)
			{
				player->Die();
				player = nullptr;
			}
		}
	}
	else
	{
		renderObjects[1].draw = false;
		renderObjects[0].draw = true;
		renderObjects[0].section = { 0,0, renderObjects[0].destRect.w, renderObjects[0].destRect.h };
	}

	GameObject::PostUpdate();
}

void FireTrap::OnCollisionEnter(PhysBody* col)
{
	if (col->gameObject->CompareTag("Player"))
	{
		if (player == nullptr)
		{
			player = (Player*)col->gameObject;
		}
	}
}

void FireTrap::OnCollisionExit(PhysBody* col)
{
	if (col->gameObject->CompareTag("Player"))
	{
		player = nullptr;
	}
}

void FireTrap::FireOn(int flashDuration,int fireDuration)
{
	if(!activeteFire)
	{
		activeteFire = true;
		this->flashDuration = flashDuration;
		this->fireDuration = fireDuration;
	}
}

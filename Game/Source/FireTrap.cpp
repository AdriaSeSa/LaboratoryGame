#include "FireTrap.h"

FireTrap::FireTrap(iPoint position, std::string name, std::string tag, Application* app)
	:GameObject(name, tag, app)
{
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
	fireFlash.speed = 0.4f;

	for (int i = 0; i < 3; i++)
	{
		fireOn.PushBack({ i * renderObjects[1].destRect.w,0,renderObjects[1].destRect.w,renderObjects[1].destRect.h });
	}
	fireOn.hasIdle = false;
	fireOn.loop = true;
	fireFlash.speed = 0.4f;
	renderObjects[1].draw = false;
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
		}
	}
}

void FireTrap::PostUpdate()
{
	if (activeteFire)
	{
		if (flashDuration > 0)
		{
			fireFlash.Update();
			renderObjects[1].draw = false;
			renderObjects[0].draw = true;
			renderObjects[0].section = fireFlash.GetCurrentFrame();
		}
		else
		{
			fireOn.Update();
			renderObjects[0].draw = false;
			renderObjects[1].draw = true;
			renderObjects[1].section = fireFlash.GetCurrentFrame();
		}
	}
	else
	{
		renderObjects[1].draw = false;
		renderObjects[0].draw = true;
		renderObjects[0].section = { 0,0, 16, 16};
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

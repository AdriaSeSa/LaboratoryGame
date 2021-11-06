#include "Saw.h"

Saw::Saw(iPoint pos, std::string name, std::string tag, Application* app) :GameObject(name, tag, app)
{	
	// Init renderObject
	InitRenderObjectWithXml();

	// Init anim;
	for (int i = 0; i < 8; i++)
	{
		anim.PushBack({ i * renderObjects[0].destRect.w,0,renderObjects[0].destRect.w,renderObjects[0].destRect.h });
	}
	anim.hasIdle = false;
	anim.speed = 0.3f;

	// Init PhysBody
	pBody = _app->physics->CreateCircle(pos.x, pos.y, renderObjects[0].destRect.w / 2, this);
	pBody->body->SetType(b2BodyType::b2_kinematicBody);
	pBody->body->GetFixtureList()->SetSensor(true);

	/*b2Filter filter;
	filter.categoryBits = 0x0004;
	filter.maskBits = 0x0002;
	filter.groupIndex = 2;
	pBody->body->GetFixtureList()->SetFilterData(filter);*/
}

void Saw::PostUpdate()
{
	anim.Update();
	renderObjects[0].section = anim.GetCurrentFrame();

	GameObject::PostUpdate();
}

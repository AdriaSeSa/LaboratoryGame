#include "SpecialPlatform.h"

SpecialPlatform::SpecialPlatform(iPoint position, std::string name, std::string tag, Application* app, int lenght, iPoint moveDistance, float moveSpeed, int stopTime) :MobilePlatform(position, name, tag, app, lenght, moveDistance, moveSpeed, stopTime)
{
	pBody->body->SetLinearVelocity({ 0, 0 });
}

void SpecialPlatform::Update()
{
	if (startMove)
	{
		MobilePlatform::Update();
	}
}

void SpecialPlatform::OnCollisionEnter(PhysBody* col)
{
	if (col->gameObject->CompareTag("Player"))
	{
		pBody->body->SetLinearVelocity({ 0, 2});
		printf("Player enter");
		startMove = true;
	}
}

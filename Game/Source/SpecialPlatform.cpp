#include "SpecialPlatform.h"
#include "FireTrap.h"

SpecialPlatform::SpecialPlatform(iPoint position, std::string name, std::string tag, Application* app, int lenght, iPoint moveDistance, float moveSpeed, int stopTime) :MobilePlatform(position, name, tag, app, lenght, moveDistance, moveSpeed, false, stopTime)
{
	pBody->body->SetLinearVelocity({ 0, 0 });

	for (int i = 0; i < lenght; i++)
	{
		//fireTraps[i] = new FireTrap({ i * renderObjects[0].destRect.w, renderObjects[0].destRect.x }, "fireTrap", "FireTrap", app);
	}
}

void SpecialPlatform::Update()
{

}

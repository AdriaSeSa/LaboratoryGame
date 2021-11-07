#include "SpecialPlatform.h"
#include "FireTrap.h"

SpecialPlatform::SpecialPlatform(iPoint position, std::string name, std::string tag, Application* app, int lenght, iPoint moveDistance, float moveSpeed, int stopTime) :MobilePlatform(position, name, tag, app, lenght, moveDistance, moveSpeed, false, stopTime)
{
	pBody->body->SetLinearVelocity({ 0, 0 });

	for (int i = 0; i < lenght; i++)
	{
		fireTraps[i] = new FireTrap({ position.x + i * renderObjects[0].destRect.w + 7, position.y + 5 }, "fireTramp", "FireTrap", app);
	}

	startPos = position;
}

void SpecialPlatform::Reset()
{
	SetPosition(startPos);
	SetLinearVelocity({ 0,0 });
	fireStep = 0;
	for (int i = 0; i < MAX_FIRENUM; i++)
	{
		if (fireTraps[i] != nullptr)
		{
			fireTraps[i]->Reset();
		}
	}
	startMove = false;
}

void SpecialPlatform::Update()
{
	for (int i = 0; i < MAX_FIRENUM; i++)
	{
		if (fireTraps[i] != nullptr)
		{
			fireTraps[i]->Update();

			if (GetPosition().y > 50 && fireStep == 0)
			{
				fireStep++;
				fireTraps[i]->FireOn(200, 100);
			}
		}
	}
}

void SpecialPlatform::PostUpdate()
{
	for (int i = 0; i < MAX_FIRENUM; i++)
	{
		if (fireTraps[i] != nullptr)
		{
			fireTraps[i]->PostUpdate();
		}
	}

	Platform::PostUpdate();
}

void SpecialPlatform::OnCollisionEnter(PhysBody* col)
{
	MobilePlatform::OnCollisionEnter(col);

	b2Vec2 myVel = GetLinearVelocity();

	for (int i = 0; i < MAX_FIRENUM; i++)
	{
		if (fireTraps[i] != nullptr)
		{
			fireTraps[i]->SetLinearVelocity(myVel);
		}
	}
}

void SpecialPlatform::CleanUp()
{
	for (int i = 0; i < MAX_FIRENUM; i++)
	{
		if (fireTraps[i] != nullptr)
		{
			delete fireTraps[i];
			fireTraps[i] = nullptr;
		}
	}
}

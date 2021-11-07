#include "MobilePlatform.h"

/// <summary>
/// 
/// </summary>
/// <param name="position"></param>
/// <param name="name"></param>
/// <param name="tag"></param>
/// <param name="app"></param>
/// <param name="lenght"></param>
/// <param name="moveDistance">La distancia que quieres desplazar (al llegar el limite se volvera a la posicion inicial), negativo = subir, positico = bajar (respecto la posicion actual), solo puedes uno de los 2 ejes</param>
/// <param name="moveSpeed"></param>
MobilePlatform::MobilePlatform(iPoint position, std::string name, std::string tag, Application* app, int lenght, iPoint moveDistance, float moveSpeed, bool loop, int stopTime)
	:Platform(position, name, tag, app, lenght)
{
	this->loop = loop;

	// Init movement
	this->speed = moveSpeed;
	this->stopTime = stopTime;
	this->countStopTime = stopTime;

	if (moveDistance.x > 0)moveDir = { 1, 0 };
	if (moveDistance.x < 0)
	{
		moveDir = { -1, 0 };
		speed *= -1;
	}
	if (moveDistance.y < 0)moveDir = { 0, -1 };
	if (moveDistance.y > 0)
	{
		moveDir = { 0, 1 };
		speed *= -1;
	}

	if (moveDistance.y < 0 || moveDistance.x > 0)
	{
		startPos = position;
		endPos = position + moveDistance;
		startVeclocity = { moveDir.x * -speed,  moveDir.y * speed };
		if (startMove)
		{
			pBody->body->SetLinearVelocity(startVeclocity);
		}
		moveState = 0;
	}
	else if (moveDistance.y > 0 || moveDistance.x < 0)
	{
		endPos = position;
		startPos = position + moveDistance;
		startVeclocity = { moveDir.x * speed,  moveDir.y * -speed };
		if (startMove)
		{
			pBody->body->SetLinearVelocity(startVeclocity);
		}
		moveState = 1;
	}
	else
	{
		moveState = 3;
	}
}

void MobilePlatform::Update()
{
	if(startMove)
	{
		iPoint myPos = GetPosition();

		if (moveState == 0)
		{
			if (myPos.y < endPos.y || myPos.x < startPos.x)
			{
				if (countStopTime > 0)
				{
					pBody->body->SetLinearVelocity({ 0,0 });
					if (loop)
					{
						countStopTime--;
					}
				
				}
				else
				{
					pBody->body->SetLinearVelocity({ moveDir.x * speed,  moveDir.y * -speed });
					moveState = 1;
					countStopTime = stopTime;
				}
			}
		}
		else if (moveState == 1)
		{
			if (myPos.y > startPos.y || myPos.x > endPos.x)
			{
				if (countStopTime > 0)
				{
					pBody->body->SetLinearVelocity({ 0,0 });
					if (loop)
					{
						countStopTime--;
					}
				}
				else
				{
					pBody->body->SetLinearVelocity({ moveDir.x * -speed,  moveDir.y * speed });
					moveState = 0;
					countStopTime = stopTime;
				}
			}
		}
	}
}

void MobilePlatform::OnCollisionEnter(PhysBody* col)
{
	if (col->gameObject->CompareTag("GroundSensor") && !startMove)
	{	
		printf("Player enter");
		pBody->body->SetLinearVelocity(startVeclocity);
		startMove = true;
	}
}

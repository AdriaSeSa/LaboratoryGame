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

	offsetY = 5;

	// Init movement
	this->speed = moveSpeed;
	this->stopTime = stopTime;
	this->countStopTime = stopTime;

	InitStates(moveDistance);
}

void MobilePlatform::InitStates(iPoint moveDistance)
{
	startPos = GetPosition();
	endPos = GetPosition() + moveDistance;
	moveDir = moveDistance.Normalize();
	distance = startPos.DistanceTo(endPos);

	startVeclocity = { moveDir.x * speed,  moveDir.y * speed };

	if (startMove)
	{
		pBody->body->SetLinearVelocity(startVeclocity);
	}
	moveState = 1;
}

void MobilePlatform::Move()
{
	pBody->body->SetLinearVelocity({ moveDir.x * speed,  moveDir.y * speed });

	int dis = startPos.DistanceTo(GetPosition());

	if (dis > distance && moveState == 0)
	{
		moveState = 1;
		countStopTime = stopTime;

		iPoint tempPos = startPos;
		startPos = endPos;
		endPos = tempPos;
	}
}

void MobilePlatform::Idle()
{
	pBody->body->SetLinearVelocity({ 0,0 });

	if (countStopTime > 0 && loop)
	{
		countStopTime--;
	}
	else
	{
		moveState = 0;
		moveDir *= -1;
	}
}

void MobilePlatform::Update()
{
	if (!startMove) return;

	if (moveState == 0) Move();

	else if (moveState == 1) Idle();
}

void MobilePlatform::OnCollisionEnter(PhysBody* col)
{
	if (col->gameObject->CompareTag("Player") && !startMove)
	{	
		pBody->body->SetLinearVelocity(startVeclocity);
		startMove = true;
		moveState = 0;
	}
}
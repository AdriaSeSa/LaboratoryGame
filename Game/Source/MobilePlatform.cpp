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
MobilePlatform::MobilePlatform(iPoint position, std::string name, std::string tag, Application* app, int lenght, iPoint moveDistance, float moveSpeed)
	:GameObject(name, tag, app)
{
	InitRenderObjectWithXml();

	if (lenght < 2) lenght = 2;
	else if (lenght > 6)lenght = 6;
	this->lenght = lenght;

	pBody = _app->physics->CreateLine({ (float)position.x, (float)position.y }, { (float)renderObjects[0].destRect.w * lenght, 0, }, this);
	pBody->body->SetType(b2_kinematicBody);

	// Init movement
	this->speed = moveSpeed;

	if (moveDistance.x > 0)moveDir = { 1, 0 };
	if (moveDistance.x < 0)
	{
		moveDir = { -1, 0 };
		speed *= -1;
	}
	if (moveDistance.y > 0)
	{
		moveDir = { 0, 1 };
		speed *= -1;
	}
	if (moveDistance.y < 0)moveDir = { 0, -1 };

	if (moveDistance.y < 0 || moveDistance.x > 0)
	{
		startPos = position;
		endPos = position + moveDistance;
		pBody->body->SetLinearVelocity({ moveDir.x * -speed,  moveDir.y * speed });
		moveState = 0;
	}
	else if (moveDistance.y > 0 || moveDistance.x < 0)
	{
		endPos = position;
		startPos = position + moveDistance;
		pBody->body->SetLinearVelocity({ moveDir.x * speed,  moveDir.y * -speed });
		moveState = 1;
	}
	else
	{
		moveState = 3;
	}
}

void MobilePlatform::Update()
{
	iPoint myPos = GetPosition();

	if (moveState == 0)
	{
		if (myPos.y < endPos.y || myPos.x < startPos.x)
		{
			pBody->body->SetLinearVelocity({ moveDir.x * speed,  moveDir.y * -speed });
			moveState = 1;
		}
	}
	else if (moveState == 1)
	{
		if (myPos.y > startPos.y || myPos.x > endPos.x)
		{
			pBody->body->SetLinearVelocity({ moveDir.x * -speed,  moveDir.y * speed });
			moveState = 0;
		}
	}
}

void MobilePlatform::PostUpdate()
{
	SDL_Rect renderSeccion = { renderObjects[0].section.x,renderObjects[0].section.y ,renderObjects[0].destRect.w,renderObjects[0].destRect.h };

	for (int i = 0; i < lenght; i++)
	{
		if (i == lenght-1)
		{
			renderSeccion.x = renderObjects[0].section.x + 2 * renderObjects[0].destRect.w;
		}
		else if (i > 0)
		{
			renderSeccion.x = renderObjects[0].section.x + renderObjects[0].destRect.w;
		}

		iPoint renderPos = GetDrawPosition();

		renderPos.x += i * renderObjects[0].destRect.w;

		// No se porque hay un offset
		renderPos.y -= 5;

		_app->renderer->AddTextureRenderQueue(renderObjects[0].texture, { renderPos.x, renderPos.y }, renderSeccion);
	}
}

#include "Platform.h"

Platform::Platform(iPoint position, std::string name, std::string tag, Application* app, int lenght = 2)
	:GameObject(name, tag, app)
{
	InitRenderObjectWithXml();

	renderObjects[0].textureCenterX = 0;
	renderObjects[0].textureCenterY = 0;

	// Set limit for lenght
	if (lenght < 2) lenght = 2;
	else if (lenght > 7)lenght = 7;
	this->lenght = lenght;

	position.x += 16 * lenght / 2;

	//pBody = _app->physics->CreateLine({ (float)position.x, (float)position.y}, { (float)renderObjects[0].destRect.w * lenght, 0, }, this);
	pBody = _app->physics->CreateRectangle(position, renderObjects[0].destRect.w * lenght, 1, this);
	pBody->body->SetType(b2_kinematicBody);
	pBody->body->GetFixtureList()->SetFriction(1000);
}

void Platform::PostUpdate()
{
	SDL_Rect renderSeccion = { renderObjects[0].section.x,renderObjects[0].section.y ,renderObjects[0].destRect.w,renderObjects[0].destRect.h };

	for (int i = 0; i < lenght; i++)
	{
		if (i == lenght - 1)
		{
			renderSeccion.x = renderObjects[0].section.x + 2 * renderObjects[0].destRect.w;
		}
		else if (i > 0)
		{
			renderSeccion.x = renderObjects[0].section.x + renderObjects[0].destRect.w;
		}

		iPoint renderPos = GetDrawPosition();

		renderPos.x += i * renderObjects[0].destRect.w - (16 * lenght / 2);

		// No se porque hay un offset
		renderPos.y -= offsetY;

		_app->renderer->AddTextureRenderQueue
		(renderObjects[0].texture, { renderPos.x, renderPos.y}, renderSeccion, renderObjects[0].scale,
			renderObjects[0].layer, renderObjects[0].orderInLayer, renderObjects[0].rotation, 
			renderObjects[0].flip, renderObjects[0].speedRegardCamera);
	}
}

void Platform::OnCollisionEnter(PhysBody* col)
{
	if (triggerPlatform)
	{
		if (col->gameObject->CompareTag("PlatformSensor"))
		{
			//printf("Player col witch platform!!!\n");
			pBody->SetSensor(true);
		}
	}
}

void Platform::OnCollisionExit(PhysBody* col)
{
	if (triggerPlatform)
	{
		if (col->gameObject->CompareTag("PlatformSensorClose"))
		{
			//printf("Player exit platform!!!\n");
			pBody->SetSensor(false);
		}
	}
}

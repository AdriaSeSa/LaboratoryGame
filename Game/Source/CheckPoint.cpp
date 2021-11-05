#include "CheckPoint.h"

CheckPoint::CheckPoint(iPoint pos, std::string name, std::string tag, Application* app) : GameObject(name, tag, app)
{
	// Physbody
	pBody = _app->physics->CreateRectangleSensor(pos, 5, 32, this);

	// RenderObject
	renderObjects[0].texture = _app->textures->Load("Assets/textures/Items/Checkpoints/Checkpoint/Checkpoint (No Flag).png");
	renderObjects[1].texture = _app->textures->Load("Assets/textures/Items/Checkpoints/Checkpoint/Checkpoint (Flag Out) (64x64).png");
	renderObjects[2].texture = _app->textures->Load("Assets/textures/Items/Checkpoints/Checkpoint/Checkpoint (Flag Idle)(64x64).png");

	for (int i = 0; i < 3; i++)
	{
		renderObjects[i].scale = 0.6f;
	}

	for (int i = 0; i < 10; i++)
	{
		idleFlag.PushBack({ 64 * i, 0, 64, 64 });
	}

	for (int i = 0; i < 26; i++)
	{
		getCheckpoint.PushBack({ 64 * i, 0, 64, 64 });
	}

	idleFlag.speed = 0.3f;
	idleFlag.hasIdle = false;

	getCheckpoint.speed = 0.5f;
	getCheckpoint.hasIdle = false;
	getCheckpoint.loop = false;

	currentState = IDLE_NO_FLAG;

}

void CheckPoint::PostUpdate()
{
	if (getCheckpoint.HasFinished()) currentState = IDLE_FLAG;

	int i = 0;
	switch (currentState)
	{
	case IDLE_NO_FLAG:
		i = 0;
		break;
	case GET_CHECKPOINT:
		i = 1;
		getCheckpoint.Update();
		break;
	case IDLE_FLAG:
		i = 2;
		idleFlag.Update();
		break;
	}

	if (renderObjects[i].texture != nullptr)
	{
		int spriteOffsetX, spriteOffsetY;
		spriteOffsetX = 11;
		spriteOffsetY = 6;

		renderObjects[i].destRect.x = GetDrawPosition().x - spriteOffsetX;
		renderObjects[i].destRect.y = GetDrawPosition().y - spriteOffsetY;

		if (i == 1)
		{
			_app->renderer->AddTextureRenderQueue(renderObjects[i].texture, { renderObjects[i].destRect.x,renderObjects[i].destRect.y },
				getCheckpoint.GetCurrentFrame(), renderObjects[i].scale, renderObjects[i].layer, renderObjects[i].orderInLayer,
				renderObjects[i].rotation, renderObjects[i].flip, renderObjects[i].speedRegardCamera);
			return;
		}
		if (i == 2)
		{
			_app->renderer->AddTextureRenderQueue(renderObjects[i].texture, { renderObjects[i].destRect.x,renderObjects[i].destRect.y },
				idleFlag.GetCurrentFrame(), renderObjects[i].scale, renderObjects[i].layer, renderObjects[i].orderInLayer,
				renderObjects[i].rotation, renderObjects[i].flip, renderObjects[i].speedRegardCamera);
			return;
		}

		_app->renderer->AddTextureRenderQueue(renderObjects[i].texture, { renderObjects[i].destRect.x,renderObjects[i].destRect.y },
			renderObjects[i].section, renderObjects[i].scale, renderObjects[i].layer, renderObjects[i].orderInLayer,
			renderObjects[i].rotation, renderObjects[i].flip, renderObjects[i].speedRegardCamera);
	}
}

void CheckPoint::OnCollision(PhysBody* col)
{
	if (col->gameObject->CompareTag("Player"))
	{
		if (currentState == IDLE_NO_FLAG)
		{
			currentState = GET_CHECKPOINT;
			_app->SaveGameRequest();
		}
	}
}

#include "CheckPoint.h"
#include "SceneGame.h"

CheckPoint::CheckPoint(iPoint pos, std::string name, std::string tag, Application* app) : GameObject(name, tag, app)
{
	// Textures
	std::string texNames[3] = { "checkPointNoFlag","checkPointFlagOut","checkPointFlagIdle" };

	for (int i = 0; i < 3; i++) InitRenderObjectWithXml(texNames[i], i); 

	// Physbody
	pBody = _app->physics->CreateRectangleSensor(pos, 5, 24, this);

	// Animation
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

	if (isActive && currentState == IDLE_NO_FLAG) currentState = GET_CHECKPOINT;

	int i = 0;
	switch (currentState)
	{
	case IDLE_NO_FLAG:
		i = 0;
		break;
	case GET_CHECKPOINT:
		isActive = true;
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
		spriteOffsetX = -5;
		spriteOffsetY = 4;

		renderObjects[i].destRect.x = GetDrawPosition().x - spriteOffsetX;
		renderObjects[i].destRect.y = GetDrawPosition().y - spriteOffsetY;

		DrawCurrentState(i);
	}
}

void CheckPoint::DrawCurrentState(int i)
{
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

void CheckPoint::OnCollisionEnter(PhysBody* col)
{
	if (col->gameObject->CompareTag("Player"))
	{
		//_app->SaveGameRequest();
		//_app->saveF.child("game_state").child("scene").child(_app->scene->currentScene->name.c_str()).child("checkPoint").attribute("isActive") = "true";
		if (currentState == IDLE_NO_FLAG)
		{
			SceneGame* scene = (SceneGame *) _app->scene->currentScene;
			scene->SaveGameFile();
			_app->SaveGameRequest();
			_app->audio->PlayFx(SFX::CHECK_POINT);
			currentState = GET_CHECKPOINT;
		}
	}
}

void CheckPoint::OnCollisionExit(PhysBody* col)
{
}

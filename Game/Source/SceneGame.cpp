#include "SceneGame.h"
#include "BackGround.h"
#include "Saw.h"
#include "Spike.h"

SceneGame::SceneGame(Application* app) :Scene(app)
{

}

bool SceneGame::Start()
{
	_app->map->Load("Upp.tmx");

	InitScene();

	backGround = new BackGround("backGround1", "BackGround", _app);
	saw = new Saw("saw", "Saw", _app);
	testSpike = new Spike({ 100,200 }, 270, "spike", "Spike", _app);

	gameObjects.add(backGround);
	gameObjects.add(saw);
	gameObjects.add(testSpike);

	return true;
}

bool SceneGame::PreUpdate()
{
	for (int i = 0; i < gameObjects.count(); i++)
	{
		gameObjects[i]->PreUpdate();
	}
	return true;
}

bool SceneGame::Update()
{
	for (int i = 0; i < gameObjects.count(); i++)
	{
		gameObjects[i]->Update();
	}
	return true;
}

bool SceneGame::PostUpdate()
{
	for (int i = 0; i < gameObjects.count(); i++)
	{
		gameObjects[i]->PostUpdate();
	}
	return true;
}

void SceneGame::InitScene()
{
	for (int i = 0; i < _app->map->mapObjects.count(); i++)
	{
		if (_app->map->mapObjects[i].id == 0)
		{
			GameObject* g = new GameObject("wall" + i, "Wall", _app);
			g->pBody = _app->physics->CreateRectangle(_app->map->mapObjects[i].position.x + 8, _app->map->mapObjects[i].position.y + 8, 16, 16);
			g->pBody->body->SetType(b2BodyType::b2_staticBody);
			gameObjects.add(g);
		}

		switch (_app->map->mapObjects[i].id)
		{
		case 0:		
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		}
	}
}

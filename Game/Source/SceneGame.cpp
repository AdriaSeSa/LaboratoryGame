#include "SceneGame.h"
#include "BackGround.h"
#include "Saw.h"
#include "Spike.h"
#include "Player.h"
#include "MobilePlatform.h"

SceneGame::SceneGame(Application* app) :Scene(app)
{

}

bool SceneGame::Start()
{
	_app->map->Load("Upp.tmx");

	InitScene();

	backGround = new BackGround("backGround1", "BackGround", _app);

	player = new Player({ 32,32 }, "player", "Player", _app);
	
	mobilePlatform1 = new MobilePlatform({ 87, 250 }, "mobilePlatform", "MobilePlatform", _app, 3, { -30, 0 }, 1.0f);

	gameObjects.add(backGround);
	gameObjects.add(player);
	gameObjects.add(mobilePlatform1);

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
			GameObject* g = new GameObject("wall", "Wall", _app);
			// +8 = offset, porque pivot de b2Body es el centro, y de tectura es izquierda superiol.
			g->pBody = _app->physics->CreateRectangle({ _app->map->mapObjects[i].position.x + 8, _app->map->mapObjects[i].position.y + 8 }, 16, 16, g);
			g->pBody->body->SetType(b2BodyType::b2_staticBody);
			gameObjects.add(g);
		}
		else if (_app->map->mapObjects[i].id == 1)
		{
			GameObject* g = new GameObject("platform", "Platform", _app);
			b2Vec2 startPos = { (float)_app->map->mapObjects[i].position.x  ,(float)_app->map->mapObjects[i].position.y };
			b2Vec2 direcction = { 16, 0 };

			g->pBody = _app->physics->CreateLine(startPos, direcction, g);
			gameObjects.add(g);
		}
		else if (_app->map->mapObjects[i].id == 2)
		{
			Saw* saw = new Saw({ _app->map->mapObjects[i].position.x ,_app->map->mapObjects[i].position.y }, "saw", "Saw", _app);
			gameObjects.add(saw);
		}
		else if (_app->map->mapObjects[i].id == 3)
		{
			// +8 = offset (depende del tile, no de mapa), porque pivot de b2Body es el centro, y de tectura es izquierda superiol.
			Spike* spike = new Spike({ _app->map->mapObjects[i].position.x + 8 ,_app->map->mapObjects[i].position.y + 8 }, _app->map->mapObjects[i].rotation, "spike", "Spike", _app);
			gameObjects.add(spike);
		}
	}
}

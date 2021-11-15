#include "SceneGame.h"
#include "BackGround.h"
#include "Saw.h"
#include "Spike.h"
#include "Player.h"
#include "MobilePlatform.h"
#include "CheckPoint.h"
#include "PowerUp.h"
#include "SpecialPlatform.h"

SceneGame::SceneGame(Application* app) :Scene(app)
{

}

bool SceneGame::Start()
{
	_app->map->Load("Level1.tmx");

	reset = false;

	InitScene();

	backGround = new BackGround("backGround1", "BackGround", _app);

	player = new Player({ 32,32 }, "player", "Player", _app);

	player->lifes = 3;

	mobilePlatform1 = new MobilePlatform({ 95, 368 }, "mobilePlatform", "MobilePlatform", _app, 2, { 0, -285 }, 1, true, 200);

	specialPlatform = new SpecialPlatform({ 185, 82 }, "specialPlatform", "MobilePlatform", _app, 7, { 0, 600 }, 0.5f, 500);

	checkPoint = new CheckPoint({ 160, 68 }, "checkpoint", "Checkpoint", _app);

	// Camera
	_app->renderer->camera->SetTarget(player);
	_app->renderer->camera->mapHeight = 640;
	_app->renderer->camera->mapWidth = 320;

	// Create test fruits
	std::string fuits[8] = { "apple","bananas","cherries","kiwi","melon","orange","pineapple","strawberry" };

	for (int i = 0; i < 8; i++)
	{
		PowerUp* g;
		g = new PowerUp({ 80 + 20 * i, 40 }, fuits[i], "PowerUp", _app);

		gameObjects.add(g);
	}

	gameObjects.add(backGround);
	gameObjects.add(player);
	gameObjects.add(mobilePlatform1);
	gameObjects.add(checkPoint);
	gameObjects.add(specialPlatform);

	_app->LoadGameRequest();

	for (int i = 0; i < gameObjects.count(); i++)
	{
		if (gameObjects[i] != nullptr)
		{
			gameObjects[i]->Start();
		}
	}

	return true;
}

bool SceneGame::PreUpdate()
{
	for (int i = 0; i < gameObjects.count(); i++)
	{
		if (gameObjects[i] != nullptr)
		{
			if (gameObjects[i]->pendingToDelete)
			{
				DestroyGameObject(gameObjects[i]);
			}
			else
			{
				gameObjects[i]->PreUpdate();
			}
		}
	}

	if (player != nullptr && player->isDead)
	{
		if (--player->lifes <= 0)
		{
			// when don't have any life
			reset = true;
			_app->SaveGameRequest();
			_app->scene->ChangeCurrentScene(2, 0);
			return true;
		}
		else
		{
			// when have life yet
			_app->LoadGameRequest();
			specialPlatform->Reset();
			player->isDead = false;
			player->PlayerAppear();
		}
	}
	return true;
}

bool SceneGame::Update()
{
	for (int i = 0; i < gameObjects.count(); i++)
	{
		if (gameObjects[i] != nullptr)
		{
			gameObjects[i]->Update();
		}
	}

	if (_app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		_app->LoadGameRequest();
	}
	if (_app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && _app->isDebug)
	{
		reset = true;
		_app->SaveGameRequest();
	}
	if (_app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		_app->scene->ChangeCurrentScene(0, 0);
	}
	return true;
}

bool SceneGame::PostUpdate()
{
	for (int i = 0; i < gameObjects.count(); i++)
	{
		if (gameObjects[i] != nullptr)
		{
			gameObjects[i]->PostUpdate();
		}
	}
	return true;
}

bool SceneGame::CleanUp()
{
	Scene::CleanUp();

	backGround = nullptr;
	player = nullptr;
	mobilePlatform1 = nullptr;
	checkPoint = nullptr;
	specialPlatform = nullptr;

	if (_app->renderer->camera != nullptr)
	{
		_app->renderer->camera->ReleaseTarget();
	}
	
	_app->map->CleanUp();

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
			g->pBody->body->GetFixtureList()->SetFriction(0);
			g->adjustToGrid = true;
			gameObjects.add(g);
		}
		else if (_app->map->mapObjects[i].id == 1)
		{
			Platform* g = new Platform({ _app->map->mapObjects[i].position.x , _app->map->mapObjects[i].position.y}, "platform", "Platform", _app, 2);
			//b2Vec2 startPos = { (float)_app->map->mapObjects[i].position.x  ,(float)_app->map->mapObjects[i].position.y };
			//b2Vec2 direcction = { 16, 0 };
			g->adjustToGrid = true;
			//g->pBody = _app->physics->CreateLine(startPos, direcction, g);
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

void SceneGame::SetSaveData()
{
	if (player != nullptr)
	{
		playerX = reset ? playerStartPos.x : player->GetPosition().x;
		playerY = reset ? playerStartPos.y : player->GetPosition().y;
	}

	reset = false;
}

void SceneGame::LoadSaveData(pugi::xml_node save)
{
	pugi::xml_node n = save;

	if (player != nullptr) player->SetPosition({ n.child("player").attribute("x").as_int(),n.child("player").attribute("y").as_int() });
}

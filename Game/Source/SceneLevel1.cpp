#include "SceneLevel1.h"
#include "BackGround.h"
#include "Saw.h"
#include "Spike.h"
#include "Player.h"
#include "MobilePlatform.h"
#include "CheckPoint.h"
#include "PowerUp.h"
#include "SpecialPlatform.h"
#include "PlayerSettings.h"

SceneLevel1::SceneLevel1(Application* app) :Scene(app)
{
	ID = 2;

	// Define platform lenght
	platformLenght = 2;
}

bool SceneLevel1::Start()
{
	_app->map->Load("Level1.tmx");

	reset = false;

	// Init scene with tmx metaDate
	InitScene();

	backGround = new BackGround("backGround1", "BackGround", _app);

	player = new Player({ 32,32 }, "player", "Player", _app);

	mobilePlatform1 = new MobilePlatform({ 95, 368 }, "mobilePlatform", "MobilePlatform", _app, 2, { 0, -285 }, 1, true, 200);

	specialPlatform = new SpecialPlatform({ 185, 82 }, "specialPlatform", "MobilePlatform", _app, 7, { 0, 600 }, 0.5f, 500);

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

bool SceneLevel1::PreUpdate()
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
		if (--_app->scene->playerSettings->playerLifes <= 0)
		{
			// when don't have any life
			reset = true;
			_app->SaveGameRequest();
			_app->scene->ChangeCurrentScene(1, 0);
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

bool SceneLevel1::Update()
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
		_app->scene->ChangeCurrentScene(2, 0);
	}
	return true;
}

bool SceneLevel1::PostUpdate()
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

bool SceneLevel1::CleanUp()
{
	Scene::CleanUp();

	if (_app->renderer->camera != nullptr)
	{
		_app->renderer->camera->ReleaseTarget();
	}
	
	_app->map->CleanUp();

	return true;
}

void SceneLevel1::SetSaveData()
{
	if (player != nullptr)
	{
		playerX = reset ? playerStartPos.x : player->GetPosition().x;
		playerY = reset ? playerStartPos.y : player->GetPosition().y;
	}

	reset = false;
}

void SceneLevel1::LoadSaveData(pugi::xml_node save)
{
	pugi::xml_node n = save;

	if (player != nullptr) player->SetPosition({ n.child("player").attribute("x").as_int(),n.child("player").attribute("y").as_int() });

	if (_app->scene->playerSettings->playerLifes != 0) return;

	_app->scene->playerSettings->playerLifes = n.child("player").attribute("lifes").as_int();
	_app->scene->playerSettings->playerScore = n.child("player").attribute("score").as_int();
}
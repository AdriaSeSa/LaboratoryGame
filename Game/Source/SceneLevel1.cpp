#include "SceneLevel1.h"
#include "Player.h"
#include "Saw.h"
#include "Spike.h"
#include "CheckPoint.h"
#include "PowerUp.h"
#include "SpecialPlatform.h"
#include "PlayerSettings.h"
#include "StaticUI.h"

SceneLevel1::SceneLevel1(Application* app, string name) :SceneGame(app, name)
{
	ID = 2;
	// Define platform lenght
	platformLenght = 2;

	playerStartPos = { 40, 560 };
}

bool SceneLevel1::Start()
{
	_app->map->Load("Level1.tmx");

	isWin = false;

	_app->scene->lastLevel = 2; // Set this as the last Level (for GameOver)

	// Init scene with tmx metaDate
	InitScene();

	player = new Player({ 32,32 }, "player", "Player", _app);

	specialPlatform = new SpecialPlatform({ 185, 85}, "specialPlatform", "MobilePlatform", _app, 7, { 0, 600 }, 0.5f, 500);

	checkPoint = new CheckPoint({ 160, 68 }, "checkPoint", "CheckPoint", _app);

	// Create test fruits
	std::string fuits[8] = { "apple","bananas","cherries","kiwi","melon","orange","pineapple","strawberry" };

	// Create test powerUps
	for (int i = 0; i < 15; i++)
	{
		PowerUp* g;
		g = new PowerUp(fruitsPos[i], fuits[rand() % 8], "PowerUp", _app);

		gameObjects.add(g);
	}

	gameObjects.add(player);
	gameObjects.add(specialPlatform);
	gameObjects.add(checkPoint);

	// init camera
	_app->renderer->camera->Init(player, 320, 640);

	// Start all gameobjects
	for (int i = 0; i < gameObjects.count(); i++)
	{
		if (gameObjects[i] != nullptr)
		{
			gameObjects[i]->Start();
		}
	}

	// Init GUI

	staticUI = new StaticUI(PlayerCharacters::VIRTUAL_GUY, _app);

	// Recargar informacion de saveF
	LoadGameFile();

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
			ResetGameFile();
			//_app->SaveGameRequest();
			_app->scene->ChangeCurrentScene(1);
			return true;
		}
		else
		{
			// when have life yet
			LoadGameFile();
			//_app->LoadGameRequest();
			specialPlatform->Reset();
			player->isDead = false;
			player->PlayerAppear();
		}
	}
	return true;
}

bool SceneLevel1::Update()
{
	Scene::Update();

	for (int i = 0; i < gameObjects.count(); i++)
	{
		if (gameObjects[i] != nullptr)
		{
			gameObjects[i]->Update();
		}
	}

	if (_app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		SaveGameFile();
	}

	staticUI->Update();

	if (isWin)
	{
		Win();
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

	staticUI->PostUpdate();

	return true;
}

bool SceneLevel1::CleanUp()
{
	Scene::CleanUp();

	if (_app->renderer->camera != nullptr)
	{
		_app->renderer->camera->ReleaseTarget();
	}

	RELEASE(staticUI);
	
	_app->map->CleanUpScene();

	return true;
}

void SceneLevel1::Win()
{
	ResetGameFile();

	_app->scene->ChangeCurrentScene(SCENES::LEVEL_2);
}
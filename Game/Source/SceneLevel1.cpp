#include "SceneLevel1.h"
#include "Player.h"
#include "Saw.h"
#include "Spike.h"
#include "CheckPoint.h"
#include "PowerUp.h"
#include "SpecialPlatform.h"
#include "PlayerSettings.h"
#include "GUI.h"

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

	// If we are resetting the scene, call reset before calling load
	//if (isReseting)
	//{
	//	reset = true;
	//	_app->SaveGameRequest();
	//}

	//_app->LoadGameRequest();

	// Start all gameobjects
	for (int i = 0; i < gameObjects.count(); i++)
	{
		if (gameObjects[i] != nullptr)
		{
			gameObjects[i]->Start();
		}
	}

	// Init GUI

	gui = new GUI(PlayerCharacters::VIRTUAL_GUY, _app);

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

	gui->Update();

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

	gui->PostUpdate();

	return true;
}

bool SceneLevel1::CleanUp()
{
	Scene::CleanUp();

	if (_app->renderer->camera != nullptr)
	{
		_app->renderer->camera->ReleaseTarget();
	}

	RELEASE(gui);
	
	_app->map->CleanUpScene();

	return true;
}

//void SceneLevel1::SetSaveData()
//{
//	if (player != nullptr)
//	{
//		playerX = reset ? playerStartPos.x : player->GetPosition().x;
//		playerY = reset ? playerStartPos.y : player->GetPosition().y;
//	}
//
//	_app->saveF.child("game_state").child("scene").child(name.c_str()).child("checkPoint").attribute("isActive") = reset ? "false" : "true";
//
//	reset = false;
//}

//void SceneLevel1::LoadSaveData(pugi::xml_node save)
//{
//	pugi::xml_node n = save;
//
//	if (player != nullptr) player->SetPosition({ n.child(name.c_str()).child("player").attribute("x").as_int(),n.child(name.c_str()).child("player").attribute("y").as_int() });
//
//	if (checkPoint != nullptr) checkPoint->isActive = _app->scene->playerSettings->reachedCheckPoint;
//	
//	if (_app->scene->playerSettings->playerLifes != 0) return;
//
//	_app->scene->playerSettings->playerLifes = n.child("player").attribute("lifes").as_int();
//	_app->scene->playerSettings->playerScore = n.child("player").attribute("score").as_int();
//}

void SceneLevel1::Win()
{
	_app->scene->ChangeCurrentScene(SCENES::LEVEL_2);
}
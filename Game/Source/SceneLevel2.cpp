#include "SceneLevel2.h"
#include "BackGround.h"
#include "Saw.h"
#include "Player.h"
#include "MobilePlatform.h"
#include "CheckPoint.h"
#include "PowerUp.h"
#include "PlayerSettings.h"
#include "BatEnemy.h"

SceneLevel2::SceneLevel2(Application* app) :Scene(app)
{
	ID = 3;
	// Define platform lenght
	platformLenght = 3;
}

bool SceneLevel2::Start()
{
	if (_app->map->Load("Level2.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;

		if (_app->map->CreateWalkabilityMap(w, h, &data)) _app->map->pathFinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}
	
	/*_app->map->pathFinding->CreatePath({ 1,36 }, { 2,27 });

	const DynArray<iPoint>* test = _app->map->pathFinding->GetLastPath();

	for (int i = 0; i < test->Count(); i++)
	{
		printf("x %d , y %d\n", test->At(i)->x, test->At(i)->y);
	}*/

	reset = false;

	// Init scene with tmx metaDate
	InitScene();

	backGround = new BackGround("backGround1", "BackGround", _app);

	player = new Player({ 32,32 }, "player", "Player", _app);

	testEnemy = new BatEnemy({ 104,72 }, player, "bat", "Bat", _app);

	mobilePlatform1 = new MobilePlatform({ 200, 500 }, "mobilePlatform", "MobilePlatform_H", _app, 2, { -120, 0 }, 1, true, 200);
	mobilePlatform1->speed = 2.5;

	// Win trigger
	winTrigger = new GameObject("winTrigger", "WinTrigger", _app);
	winTrigger->pBody = _app->physics->CreateRectangleSensor({ 320, 40 }, 20, 48, winTrigger);

	// Camera
	_app->renderer->camera->SetTarget(player);
	_app->renderer->camera->mapHeight = 640;
	_app->renderer->camera->mapWidth = 320;

	gameObjects.add(backGround);
	gameObjects.add(player);
	gameObjects.add(mobilePlatform1);
	gameObjects.add(winTrigger);
	gameObjects.add(testEnemy);
	
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

bool SceneLevel2::PreUpdate()
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
			player->isDead = false;
			player->PlayerAppear();
		}
	}

	return true;
}

bool SceneLevel2::Update()
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
		_app->scene->ChangeCurrentScene(SCENES::LEVEL_2, 0);
	}

	if(isWin)
	{
		Win();
	}

	return true;
}

bool SceneLevel2::PostUpdate()
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

bool SceneLevel2::CleanUp()
{
	Scene::CleanUp();

	if (_app->renderer->camera != nullptr)
	{
		_app->renderer->camera->ReleaseTarget();
	}

	_app->map->CleanUp();

	return true;
}

void SceneLevel2::SetSaveData()
{
	if (player != nullptr)
	{
		playerX = reset ? playerStartPos.x : player->GetPosition().x;
		playerY = reset ? playerStartPos.y : player->GetPosition().y;
	}

	reset = false;
}

void SceneLevel2::LoadSaveData(pugi::xml_node save)
{
	pugi::xml_node n = save;

	if (player != nullptr) player->SetPosition({ n.child("player").attribute("x").as_int(),n.child("player").attribute("y").as_int() });

	if (_app->scene->playerSettings->playerLifes != 0) return;

	_app->scene->playerSettings->playerLifes = n.child("player").attribute("lifes").as_int();
	_app->scene->playerSettings->playerScore = n.child("player").attribute("score").as_int();
}

void SceneLevel2::Win()
{
	_app->scene->ChangeCurrentScene(SCENES::MAIN_MENU, 0);
}

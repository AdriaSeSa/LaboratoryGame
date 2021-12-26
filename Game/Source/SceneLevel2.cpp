#include "SceneLevel2.h"
#include "Player.h"
#include "Saw.h"
#include "CheckPoint.h"
#include "PowerUp.h"
#include "BatEnemy.h"
#include "ChameleonEnemy.h"
#include "PlayerSettings.h"
#include "GUI.h"

SceneLevel2::SceneLevel2(Application* app, string name) :Scene(app, name)
{
	ID = 3;
	// Define platform lenght
	platformLenght = 3;
}

bool SceneLevel2::Start(bool isReseting)
{
	_app->scene->lastLevel = 3; // Set this as the last Level (for GameOver)

	// Load Map
	if (_app->map->Load("Level2.tmx") == true)
	{
		int w, h;
		uchar* data = nullptr;

		if (_app->map->CreateWalkabilityMap(w, h, &data)) _app->map->pathFinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}
	
	reset = false;

	isWin = false;

	player = new Player({ 32,32 }, "player", "Player", _app);

	// Create test fruits
	std::string fuits[8] = { "apple","bananas","cherries","kiwi","melon","orange","pineapple","strawberry" };

	// Create test powerUps
	for (int i = 0; i < 15; i++)
	{
		PowerUp* g;
		g = new PowerUp(fruitsPos[i], fuits[rand() % 8], "PowerUp", _app);

		gameObjects.add(g);
	}

	// Init scene with tmx metaDate
	InitScene();

	checkPoint = new CheckPoint({ 144, 308 }, "checkPoint", "CheckPoint", _app);

	// Init camera
	_app->renderer->camera->Init(player, 320, 640);

	gameObjects.add(player);
	gameObjects.add(checkPoint);
	
	// If we are resetting the scene, call reset before calling load
	if (isReseting)
	{
		reset = true;
		_app->SaveGameRequest();
		SetEnemiesData();
	}
	_app->LoadGameRequest();

	// Init Enemies
	LoadEnemies();

	// Start all gameobjects
	for (int i = 0; i < gameObjects.count(); i++)
	{
		if (gameObjects[i] != nullptr)
		{
			gameObjects[i]->Start();
		}
	}

	// Init GUI
	gui = new GUI(0, _app);

	return true;
}

/// <summary>
/// Init enemic for this scene with xml
/// </summary>
void SceneLevel2::LoadEnemies()
{
	// Get EnemySave node in Xml
	pugi::xml_node saveNode = _app->saveF.child("game_state").child("scene").child(name.c_str()).child("enemies");

	// Get Enemy node in Xml
	pugi::xml_node enemicNode = _app->scene->config.child(name.c_str()).child("enemies");

	// Iterate the node
	for (enemicNode = enemicNode.first_child(); enemicNode; enemicNode = enemicNode.next_sibling())
	{
		// get the enemic name
		string enemyName = enemicNode.name();

		// get enemic init position
		iPoint position = { enemicNode.attribute("positionX").as_int(0),enemicNode.attribute("positionY").as_int(0) };	

		int enemyID = enemicNode.attribute("ID").as_int(0);

		string nameID = enemyName.c_str() + std::to_string(enemyID);

		int lifes = saveNode.child(nameID.c_str()).attribute("lifes").as_int();

		bool isAlive = saveNode.child(nameID.c_str()).attribute("isAlive").as_bool();

		// Create diferent enemic depend name
		if (enemyName == "bat")
		{
			// Create BatEnemy and add to gameobjects list
			if (isAlive) gameObjects.add(new BatEnemy(position, player, enemyID, 1, "bat", "Bat", _app));
		}
		else if (enemyName == "chameleon")
		{
			// Create ChameleonEnemy and add to gameobjects list
			if (isAlive) gameObjects.add(new ChameleonEnemy(position, player, enemyID, lifes, "chameleon", "Chameleon", _app));
		}
	}
}

void SceneLevel2::SetEnemiesData()
{
	// If doc is not loaded
	if(!_app->saveF) _app->saveF.load_file(SAVE_STATE_FILENAME);

	// Enemies
	pugi::xml_node saveNode = _app->saveF.child("game_state").child("scene").child(name.c_str()).child("enemies");

	// Reset all enemies data
	for (saveNode = saveNode.first_child(); saveNode; saveNode = saveNode.next_sibling())
	{
		string enemyName = saveNode.name();
		enemyName.pop_back();
		enemyName.pop_back();

		if (reset)
		{
			saveNode.attribute("lifes") = enemyName == "bat" ? 1 : 2;
			saveNode.attribute("isAlive") = "true";
		}
		else
		{
			saveNode.attribute("lifes") = "0";
			saveNode.attribute("isAlive") = "false";
		}
	}

	saveNode = _app->saveF.child("game_state").child("scene").child(name.c_str()).child("enemies");

	for (int i = 0; i < gameObjects.count(); i++)
	{
		if (gameObjects[i]->name == "bat" || gameObjects[i]->name == "chameleon")
		{
			string enemyName = gameObjects[i]->name;

			Enemy* tempEnemy = (Enemy*)gameObjects[i];

			int enemyID = tempEnemy->ID;

			int enemyLifes = !reset ? tempEnemy->life : enemyName == "bat" ? 1 : 2;

			saveNode.child((enemyName.c_str() + std::to_string(enemyID)).c_str()).attribute("lifes") = enemyLifes;
			saveNode.child((enemyName.c_str() + std::to_string(enemyID)).c_str()).attribute("isAlive") = enemyLifes > 0;
		}
	}
}

bool SceneLevel2::PreUpdate()
{
	for (int i = 0; i < gameObjects.count(); i++)
	{
		if (gameObjects[i] != nullptr)
		{
			if (gameObjects[i]->pendingToDelete)
			{
				gameObjects[i]->CleanUp();
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
			player->PlayerAppear();
			player->isDead = false;
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
		//_app->LoadGameRequest();
	}
	if (_app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		//reset = true;
		//_app->SaveGameRequest();
	}

	gui->Update();

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

	gui->PostUpdate();

	return true;
}

bool SceneLevel2::CleanUp()
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

void SceneLevel2::SetSaveData()
{
	if (player != nullptr)
	{
		playerX = reset ? playerStartPos.x : player->GetPosition().x;
		playerY = reset ? playerStartPos.y : player->GetPosition().y;
	}
	
	_app->saveF.child("game_state").child("scene").child(name.c_str()).child("checkPoint").attribute("isActive") = reset ? "false" : "true";

	SetEnemiesData();

	reset = false;
}

void SceneLevel2::LoadSaveData(pugi::xml_node save)
{
	pugi::xml_node n = save;

	if (player != nullptr) player->SetPosition({ n.child(name.c_str()).child("player").attribute("x").as_int(),n.child(name.c_str()).child("player").attribute("y").as_int() });

	if (_app->scene->playerSettings->playerLifes != 0) return;

	_app->scene->playerSettings->playerLifes = n.child("player").attribute("lifes").as_int();
	_app->scene->playerSettings->playerScore = n.child("player").attribute("score").as_int();
}

void SceneLevel2::Win()
{
	_app->scene->ChangeCurrentScene(SCENES::MAIN_MENU, 0);
}
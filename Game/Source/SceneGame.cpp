#include "SceneGame.h"
#include "BackGround.h"

SceneGame::SceneGame(Application* app) :Scene(app)
{

}

bool SceneGame::Start()
{
	_app->map->Load("Upp.tmx");

	backGround = new BackGround("BackGround1", "BackGround", _app);

	gameObjects.add(backGround);

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

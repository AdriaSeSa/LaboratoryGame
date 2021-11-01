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

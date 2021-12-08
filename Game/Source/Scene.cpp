#include "Scene.h"
#include "MobilePlatform.h"
#include "Saw.h"
#include "Spike.h"
#include "CheckPoint.h"
#include "FireTrap.h"

Scene::Scene(Application* app)
{
	_app = app;
}

Scene::~Scene()
{
}

bool Scene::InitScene()
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
			Platform* g = new Platform({ _app->map->mapObjects[i].position.x , _app->map->mapObjects[i].position.y }, "platform", "Platform", _app, platformLenght);
			g->adjustToGrid = true;
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
		else if (_app->map->mapObjects[i].id == 4)
		{
			FireTrap* ft = new FireTrap({ _app->map->mapObjects[i].position.x + 8 ,_app->map->mapObjects[i].position.y }, "fireTrap2", "FireTrap", _app);
			gameObjects.add(ft);
		}
		else if (_app->map->mapObjects[i].id == 5)
		{
			//CheckPoint* chekPoint = new CheckPoint({ _app->map->mapObjects[i].position.x ,_app->map->mapObjects[i].position.y + 4}, "checkpoint", "Checkpoint", _app);
			//gameObjects.add(chekPoint);
		}
	}

	return true;
}

bool Scene::Start(bool reset)
{
	return true;
}

bool Scene::PreUpdate()
{
	return true;
}

bool Scene::Update()
{
	return true;
}

bool Scene::PostUpdate()
{
	return true;
}

bool Scene::CleanUp()
{
	for (int i = 0; i < gameObjects.count(); i++)
	{
		if (i == 258)
		{
			printf("i");
			GameObject* g = gameObjects[i];
		}
		if (gameObjects[i])
		{		
			gameObjects[i]->CleanUp();
		}
	}

	gameObjects.clearPtr();

	for (int i = 0; i < sceneTextures.count(); i++)
	{
		if(sceneTextures[i])
		{
			_app->textures->Unload(sceneTextures[i]);
		}
	}

	return true;
}

void Scene::DestroyGameObject(GameObject* gameObject)
{
	int index = gameObjects.find(gameObject);

	if (index >= 0)
	{
		gameObjects.delPtr(gameObjects.At(index));
	}
}

void Scene::SetSaveData()
{
}

void Scene::LoadSaveData(pugi::xml_node save)
{
}
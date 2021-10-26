#include "Scene.h"

#include <time.h>


ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "scene";
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init(pugi::xml_node& config)
{
	return true;
}

bool ModuleScene::Start()
{

	currentScene = nullptr;

	bool ret = true;

	if (currentScene == nullptr)
	{
		return ret;
	}

	currentScene->Start();

	return ret;
}

UpdateStatus ModuleScene::PreUpdate()
{
	if (currentScene == nullptr)
	{
		return UpdateStatus::UPDATE_CONTINUE;
	}

	currentScene->PreUpdate();

	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleScene::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		ChangeCurrentScene(0, 0);
	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		ChangeCurrentScene(1, 0);
	}
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		ChangeCurrentScene(2, 0);
	}
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		ChangeCurrentScene(3, 0);
	}
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		ChangeCurrentScene(4, 0);
	}


	if (currentScene == nullptr)
	{
		return UpdateStatus::UPDATE_CONTINUE;
	}

	currentScene->Update();

	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleScene::PostUpdate()
{
	if (currentScene == nullptr)
	{
		return UpdateStatus::UPDATE_CONTINUE;
	}

	currentScene->PostUpdate();

	return UpdateStatus::UPDATE_CONTINUE;
}

//CleanUp current scene, change current scene (index), Start current Scene
bool ModuleScene::ChangeCurrentScene(uint index, int frames)
{
	this->index = index;
	currentScene->CleanUp();
	currentScene = scenes[index];
	currentScene->Start();

	return true;
}



bool ModuleScene::CleanUp()
{
	for (int i = 0; i < SCENES_NUM; i++)
	{
		if (scenes[i] != nullptr)
		{
			scenes[i]->CleanUp();	//CleanUp all scenes (in case the Application is shut down)
			delete scenes[i];
			scenes[i] = nullptr;
		}
	}

	return true;
}

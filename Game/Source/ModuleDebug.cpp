#include "ModuleDebug.h"
#include "Application.h"
#include "ModuleScene.h"
#include "SceneGame.h"

ModuleDebug::ModuleDebug(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "debug";
}

ModuleDebug::~ModuleDebug()
{
}

UpdateStatus ModuleDebug::PreUpdate()
{
	return UPDATE_CONTINUE;
}

UpdateStatus ModuleDebug::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		App->scene->ChangeCurrentSceneRequest(2);
	}
	else if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		App->scene->ChangeCurrentSceneRequest(3);
	}
	else if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		App->scene->ChangeCurrentSceneRequest(App->scene->currentSceneState);
	}
	else if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		currentSceneIndex = (int)App->scene->currentSceneState;

		if (currentSceneIndex > 1)
		{
			SceneGame* sceneGame = (SceneGame*)App->scene->scenes[currentSceneIndex];

			GetSaveData();

			sceneGame->SaveGameFile();
		}		
	}
	else if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		LoadSave();
	}	
	else if(App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		debugViewGUIBounds = !debugViewGUIBounds;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		debugCollisionView = !debugCollisionView;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		// Code in player
	}
	else if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		debugHighFramerate = !debugHighFramerate;

		if (debugHighFramerate) App->frameTime = 1.0f / 60.0f;
		else  App->frameTime = 1.0f / 30.0f;
	}

	else if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		int scene = (int)App->scene->currentSceneState;

		if (scene != 0 && scene != 1)
		{
			debugPause = !debugPause;
		}
	}

	return UPDATE_CONTINUE;
}

UpdateStatus ModuleDebug::PostUpdate()
{
	return UPDATE_CONTINUE;
}

void ModuleDebug::GetSaveData()
{
	pugi::xml_node node = App->saveF.child("game_state").child("debug").child("lastState");

	int scene = currentSceneIndex;

	node.attribute("scene") = scene;

	node.attribute("isSaved") = true;

	App->SaveGameRequest();
}

void ModuleDebug::LoadSave()
{
	pugi::xml_node node = App->saveF.child("game_state").child("debug").child("lastState");

	bool isSaved = node.attribute("isSaved").as_bool("false");

	if (isSaved)
	{
		int scene = node.attribute("scene").as_int(0);

		App->scene->ChangeCurrentSceneRequest(scene);
	}

	App->SaveGameRequest();
}

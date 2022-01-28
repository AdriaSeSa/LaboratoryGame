#ifndef __MODULESCENE_H__
#define __MODULESCENE_H__

#include "Module.h"
#include "Globals.h"
#include "PlayerSettings.h"

#define SCENES_NUM 9

class Scene;

enum SCENES
{
	MAIN_MENU,
	GAME_OVER,
	LEVEL_1,
	LEVEL_2
};

class ModuleScene : public Module
{
public:

	// Constructor
	ModuleScene(Application* app, bool start_enabled = true);

	// Destructor
	~ModuleScene();

	bool Init(pugi::xml_node&);

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start();

	UpdateStatus PreUpdate() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	UpdateStatus Update();

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	UpdateStatus PostUpdate();

	UpdateStatus EndUpdate() override;

	bool ChangeCurrentSceneRequest(uint index);

	void GetSaveData(pugi::xml_document& save) override;

	void LoadSaveData(pugi::xml_document& save) override;

	void ResetPlayerSettings();

	bool CleanUp();

	void DebugChangeScene();

private:
	bool ChangeCurrentScene();

	SDL_Rect fadeRect = { -160,160,320,1280 };


public:

	int index = 2;

	Scene* currentScene = nullptr;

	SCENES currentSceneState = MAIN_MENU;

	Scene* scenes[SCENES_NUM] = { nullptr };

	SDL_Scancode debugKeys[SCENES_NUM] = { SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4
	, SDL_SCANCODE_5 , SDL_SCANCODE_6, SDL_SCANCODE_7, SDL_SCANCODE_8, SDL_SCANCODE_9};

	bool isChangingScene = false;
	int changeTo = -1;

	int playerX = 0, playerY = 0;

	int lastLevel = 0;

	PlayerSettings* playerSettings = nullptr;
};

#endif // __MODULESCENE_H__

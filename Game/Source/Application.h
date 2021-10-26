#pragma once

#include "p2List.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleScene.h"
#include "ModuleUI.h"
#include "ModuleMap.h"

#include "Timer.h"

#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"

#define FPS 90

class Application
{
public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModulePhysics* physics;
	ModuleScene* scene;
	ModuleUI* ui;
	ModuleMap* map;

	Timer globalTime;

	bool isDebug = true;

	std::string title;
	std::string organization;

	mutable bool saveGameRequested;
	bool loadGameRequested;

private:

	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	UpdateStatus Update();
	bool CleanUp();

	void LoadGameRequest();
	void SaveGameRequest() const;


	pugi::xml_node LoadConfig(pugi::xml_document&) const;


	bool FullScreenDesktop = true;

private:

	// Load / Save
	bool LoadGame();
	bool SaveGame() const;

	void AddModule(Module* mod);
};
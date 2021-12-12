#pragma once
#include "Module.h"

class ModuleDebug : public Module
{
public:
	ModuleDebug(Application* app, bool start_enabled = true);
	~ModuleDebug();

	UpdateStatus PreUpdate() override;
	UpdateStatus Update() override;
	UpdateStatus PostUpdate() override;

	void GetSaveData(pugi::xml_document& save) override;
	void LoadSave(pugi::xml_document& save);

	int currentSceneIndex;

	// Project debug variable
	bool debugCamera = false;
	bool debugCollisionView = false;
	bool debugGodMode = false;
	bool debugHighFramerate = true;

	// Our debug variable
	bool debugTakeObject = false;
	bool debugPause = false;
};

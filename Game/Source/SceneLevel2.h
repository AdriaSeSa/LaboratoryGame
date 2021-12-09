#pragma once
#include "Scene.h"

class Saw;
class Player;
class CheckPoint;
class PowerUp;

class SceneLevel2 : public Scene
{
public:
	SceneLevel2(Application* app, string name);

	bool Start(bool isReseting = false) override;

	void LoadEnemies();

	bool PreUpdate() override;

	bool Update() override;

	bool PostUpdate() override;

	bool CleanUp() override;

	void SetSaveData() override;

	void LoadSaveData(pugi::xml_node save) override;

	void Win();

public:

	Player* player = nullptr;

	CheckPoint* checkPoint = nullptr;

	iPoint playerStartPos = { 40, 567 };

	bool reset = false;
};
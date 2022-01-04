#pragma once
#include "SceneGame.h"

class Saw;
class Player;
class CheckPoint;
class PowerUp;
class StaticUI;

class SceneLevel2 : public SceneGame
{
public:
	SceneLevel2(Application* app, string name);

	bool Start() override;

	void LoadEnemies();

	void SetEnemiesData();

	bool PreUpdate() override;

	bool Update() override;

	bool PostUpdate() override;

	bool CleanUp() override;

	//void SetSaveData() override;

	void SaveGameFile() override;

	void ResetGameFile() override;

	void LoadSaveData(pugi::xml_node save) override;

	void Win();

public:

	//Player* player = nullptr;

	//CheckPoint* checkPoint = nullptr;

	//GUI* gui = nullptr;

	iPoint fruitsPos[8] = { { 100, 524 }, { 160, 524 }, { 220, 524 }, { 142, 440 }, { 185, 390 }, { 290, 360 }, { 290, 330 }, { 290, 300 } };
};
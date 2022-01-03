#pragma once
#include "SceneGame.h"

class Saw;
class Spike;
class Player;
class CheckPoint;
class PowerUp;
class SpecialPlatform;
class GUI;

class SceneLevel1 :  public SceneGame
{
public:
	SceneLevel1(Application* app,string name);

	bool Start() override;

	bool PreUpdate() override;

	bool Update() override;

	bool PostUpdate() override;

	bool CleanUp() override;

	//void SetSaveData() override;

	//void LoadSaveData(pugi::xml_node save) override;

	void Win();

public:
	//Player* player = nullptr;

	//CheckPoint* checkPoint = nullptr;

	SpecialPlatform* specialPlatform = nullptr;

	//GUI* gui = nullptr;

	iPoint fruitsPos[15] = { {120, 510}, {60, 465}, {120, 425}, {100, 400}, {70, 380}, {55, 400}, {32, 325}, {32, 265},  {32, 165}, {32, 95}, {240, 120}, {210, 180}, {260, 260}, {200, 340}, {260, 400} };
};

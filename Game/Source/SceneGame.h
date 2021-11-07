#pragma once
#include "Scene.h"

class BackGround;
class Saw;
class Spike;
class Player;
class MobilePlatform;
class CheckPoint;
class PowerUp;
class SpecialPlatform;

class SceneGame :  public Scene
{
public:
	SceneGame(Application* app);

	bool Start() override;

	bool PreUpdate() override;

	bool Update() override;

	bool PostUpdate() override;

	bool CleanUp() override;

	void InitScene();

	void SetSaveData() override;

	void LoadSaveData(pugi::xml_node save) override;

public:
	BackGround* backGround = nullptr;
	Player* player = nullptr;
	MobilePlatform* mobilePlatform1 = nullptr;
	CheckPoint* checkPoint = nullptr;
	SpecialPlatform* specialPlatform = nullptr;

	iPoint playerStartPos = { 40, 567 };
	bool reset = false;
};

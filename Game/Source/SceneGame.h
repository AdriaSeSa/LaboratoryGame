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

	void InitScene();

	void SetSaveData();

public:
	BackGround* backGround = nullptr;
	Player* player = nullptr;
	MobilePlatform* mobilePlatform1 = nullptr;
	CheckPoint* checkPoint = nullptr;
	SpecialPlatform* specialPlatform = nullptr;
};

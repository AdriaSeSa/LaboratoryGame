#pragma once
#include "Scene.h"

class BackGround;
class Saw;
class Player;
class MobilePlatform;
class CheckPoint;
class PowerUp;
class BatEnemy;

class SceneLevel2 : public Scene
{
public:
	SceneLevel2(Application* app);

	bool Start(bool isReseting = false) override;

	bool PreUpdate() override;

	bool Update() override;

	bool PostUpdate() override;

	bool CleanUp() override;

	void SetSaveData() override;

	void LoadSaveData(pugi::xml_node save) override;

	void Win();

public:

	BackGround* backGround = nullptr;

	Player* player = nullptr;

	MobilePlatform* mobilePlatform1 = nullptr;

	CheckPoint* checkPoint = nullptr;

	iPoint playerStartPos = { 40, 567 };

	bool reset = false;

	GameObject* winTrigger;

	BatEnemy* testEnemy = nullptr;
};
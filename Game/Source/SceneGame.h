#pragma once
#include "Scene.h"

class BackGround;
class Saw;
class Spike;
class Player;

class SceneGame :  public Scene
{
public:
	SceneGame(Application* app);

	bool Start() override;

	bool PreUpdate() override;

	bool Update() override;

	bool PostUpdate() override;

	void InitScene();

public:
	BackGround* backGround;
	Player* player;
};


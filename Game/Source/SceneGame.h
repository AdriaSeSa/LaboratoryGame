#pragma once
#include "Scene.h"

class BackGround;
class Saw;

class SceneGame :  public Scene
{
public:
	SceneGame(Application* app);

	bool Start() override;

	bool PreUpdate() override;

	bool Update() override;

	bool PostUpdate() override;

public:
	BackGround* backGround;
	Saw* saw;
};


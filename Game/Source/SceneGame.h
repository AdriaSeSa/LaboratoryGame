#pragma once
#include "Scene.h"
class SceneGame :  public Scene
{
public:
	SceneGame(Application* app);

	bool Start() override;
};


#pragma once
#include "Scene.h"
class SceneGameOver : public Scene
{
public:
	SceneGameOver(Application* app);

	bool Start() override;

	bool Update() override;
	bool PostUpdate() override;

	bool CleanUp() override;
private:
	iPoint arrowPos;
};

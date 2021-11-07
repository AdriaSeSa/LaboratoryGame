#pragma once
#include "Scene.h"

class BackGround;
class SceneMainMenu : public Scene
{
public:
	SceneMainMenu(Application* app);

	bool Start() override;

	bool Update() override;
	bool PostUpdate() override;

private:
	iPoint arrowPos;
	BackGround* bg;

	int logoY = -100;

	void SelectDifficulty();
};


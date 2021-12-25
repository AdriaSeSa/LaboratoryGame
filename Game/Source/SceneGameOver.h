#pragma once
#include "Scene.h"
class BackGround;
class SceneGameOver : public Scene
{
public:
	SceneGameOver(Application* app);

	bool Start(bool reset = false) override;

	bool Update() override;
	bool PostUpdate() override;

	void SetSaveData() override;

private:
	iPoint arrowPos = { 0,0 };
	BackGround* bg = nullptr;
	SDL_Texture* mainMenu = nullptr;
	SDL_Texture* arrow = nullptr;

	iPoint playerStartPos = { 40, 567 };

	int t = 0;
};

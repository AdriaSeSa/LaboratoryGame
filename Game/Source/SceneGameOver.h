#pragma once
#include "Scene.h"
class BackGround;
class PanelGameOver;

class SceneGameOver : public Scene
{
public:
	SceneGameOver(Application* app);

	bool Start() override;

	bool Update() override;
	bool PostUpdate() override;

	bool CleanUp() override;

	//void SetSaveData() override;

private:
	iPoint arrowPos = { 0,0 };
	BackGround* bg = nullptr;
	SDL_Texture* mainMenu = nullptr;
	SDL_Texture* arrow = nullptr;

	PanelGameOver* panel;

	iPoint playerStartPos = { 40, 567 };
};

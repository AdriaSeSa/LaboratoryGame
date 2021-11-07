#pragma once
#include "Scene.h"
class BackGround;
class SceneGameOver : public Scene
{
public:
	SceneGameOver(Application* app);

	bool Start() override;

	bool Update() override;
	bool PostUpdate() override;

private:
	iPoint arrowPos;
	BackGround* bg;
	SDL_Texture* mainMenu;
	SDL_Texture* arrow;
};

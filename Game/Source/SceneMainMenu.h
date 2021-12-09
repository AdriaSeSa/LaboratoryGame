#pragma once
#include "Scene.h"

class BackGround;

class SceneMainMenu : public Scene
{
public:
	SceneMainMenu(Application* app);

	bool Start(bool reset = false) override;

	void InitTextures();

	void MoveArrow();

	bool Update() override;

	bool PostUpdate() override;

	void ChangeScreen(int screen);

	void SelectOption();

	void ScreenOptions0();

	void ScreenOptions1();

	void ScreenOptions2();

	bool CleanUp() override;

private:
	int currentArrowPos;

	int currentScreen;

	List<iPoint> arrowPositions;

	BackGround* bg;

	int logoY = -100;

	SDL_Texture* mainMenu;

	SDL_Texture* arrow;

	SDL_Texture* logo;

	List<SDL_Texture*> currentTextures;

	List<SDL_Texture*> mainMenuTextures;

	List<SDL_Texture*> selectLevelTextures;

	bool exit;
};


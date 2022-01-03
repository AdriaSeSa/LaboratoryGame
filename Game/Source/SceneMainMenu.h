#pragma once
#include "Scene.h"
#include "Animation.h"

class BackGround;

class SceneMainMenu : public Scene
{
public:
	SceneMainMenu(Application* app);

	bool Start() override;

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
	int currentArrowPos = 0;

	int currentScreen = 0;

	int arrowAnimOffset = 0;

	bool arrowAnimLeft = false;

	List<iPoint> arrowPositions;

	BackGround* bg = nullptr;

	int logoY = -100;

	SDL_Texture* arrow = nullptr;

	SDL_Rect arrowSection = {0,0,0,0};

	SDL_Texture* sArrow = nullptr;

	List<RenderObject> currentTextures;

	List<RenderObject> mainMenuTextures;

	List<RenderObject> selectLevelTextures;

	Animation logoAnim;

	bool exit = false;

	int arrowCounter = 20;
};


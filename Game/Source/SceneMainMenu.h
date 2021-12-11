#pragma once
#include "Scene.h"
#include "Animation.h"

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

	int arrowAnimOffset;

	bool arrowAnimLeft = false;

	List<iPoint> arrowPositions;

	BackGround* bg;

	int logoY = -100;

	SDL_Texture* arrow;

	SDL_Rect arrowSection;

	SDL_Texture* sArrow;

	List<RenderObject> currentTextures;

	List<RenderObject> mainMenuTextures;

	List<RenderObject> selectLevelTextures;

	Animation logoAnim;

	bool exit;

	int arrowCounter = 20;
};


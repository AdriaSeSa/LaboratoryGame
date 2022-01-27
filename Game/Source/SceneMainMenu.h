#pragma once
#include "Scene.h"
#include "Animation.h"

class BackGround;
class GUIPanel;

class SceneMainMenu : public Scene
{
public:
	SceneMainMenu(Application* app);

	bool Start() override;

	void InitTextures();

	bool Update() override;

	bool PostUpdate() override;

	void ChangeScreen(int screen);

	void SelectOption();

	bool CleanUp() override;

	bool exit = false;

private:

	int currentScreen = 0;

	BackGround* bg = nullptr;

	int logoY = -100;

	List<RenderObject> currentTextures;

	List<RenderObject> mainMenuTextures;

	List<RenderObject> selectLevelTextures;

	List<GUIPanel*> panels;

	Animation logoAnim;
};


#pragma once
#include "Scene.h"

class Player;
class CheckPoint;
class StaticUI;

class SceneGame : public Scene
{
public:
	SceneGame(Application* app, string name = "null");

public:
	Player* player = nullptr;
	CheckPoint* checkPoint = nullptr;
	StaticUI* staticUI = nullptr;

protected:
	iPoint playerStartPos = { 40, 560 };

public:
	virtual void SaveGameFile();
	// Only GameOver resets information
	virtual void ResetGameFile();

	virtual void LoadGameFile();
};


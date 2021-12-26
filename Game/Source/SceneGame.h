#pragma once
#include "Scene.h"

class Player;
class CheckPoint;
class GUI;


class SceneGame : public Scene
{
public:
	SceneGame(Application* app, string name = "null");

public:

	// Only GameOver resets information
	virtual void SaveGameFile();
	virtual void ResetGameFile();

};


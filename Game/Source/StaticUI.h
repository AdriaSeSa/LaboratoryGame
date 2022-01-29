#pragma once

#include "Application.h"

#define CHARACTERS_NUM 1

enum PlayerCharacters
{
	VIRTUAL_GUY
};

class StaticUI
{
private:
	SDL_Texture* gui = nullptr;

	SDL_Texture* hearths[3] = { nullptr };

	SDL_Texture* skill[CHARACTERS_NUM] = { nullptr };

	SDL_Rect hearthsSection[3] = { {0,0,0,0}, {0,0,0,0}, {0,0,0,0} };

	SDL_Rect skillSection = {0,0,0,0};

	PlayerCharacters currentCharacter;

	Application* _app = nullptr;

	uint scoreUI = 0, coolDownUI = 0, highScoreUI = 0;
	uint secondsUI = 0, minutesUI = 0;

	float seconds = 0, minutes = 0;

public:
	StaticUI(int character, Application* app);

	~StaticUI();

	void Update();

	void PostUpdate();
};


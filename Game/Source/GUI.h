#pragma once

#include "Application.h"

#define CHARACTERS_NUM 1

enum PlayerCharacters
{
	VIRTUAL_GUY
};

class GUI
{
private:
	SDL_Texture* gui = nullptr;

	SDL_Texture* hearths[3] = { nullptr };

	SDL_Texture* skill[CHARACTERS_NUM] = { nullptr };

	SDL_Rect hearthsSection[3];

	SDL_Rect skillSection;

	PlayerCharacters currentCharacter;

	Application* _app;

	uint scoreUI, coolDownUI;

public:
	GUI(int character, Application* app);

	~GUI();

	void Update();

	void PostUpdate();
};


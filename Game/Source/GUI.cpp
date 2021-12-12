#include "GUI.h"

GUI::GUI(int character, Application* app)
{
	// Get current Character
	currentCharacter = (PlayerCharacters)character;

	// Init App
	_app = app;

	coolDownUI = -1;

	// Initialize textures
	gui = _app->textures->Load("Assets/textures/UI/UIOverlay.png");

	for (int i = 0; i < 3; i++)
	{
		hearths[i] = _app->textures->Load("Assets/textures/UI/Hearth(28x24).png");
		hearthsSection[i] = { 28,0, 28, 24 };	// Empty Hearths
	}

	skill[0] = _app->textures->Load("Assets/textures/UI/VGAbility.png");

	// Initialize sections

	for (int i = 0; i < _app->scene->playerSettings->playerLifes; i++)
	{
		// Full Hearths
		hearthsSection[i] = { 0,0, 28, 24 };
	}

	//skillSection = 

	// Score UI
	scoreUI = _app->ui->CreateUI(_app->scene->playerSettings->playerScore, 200, 16, 0.5f, 2,2);
}

GUI::~GUI()
{
	_app->ui->DestroyUI(scoreUI);
	if (coolDownUI != -1)
	{
		_app->ui->DestroyUI(coolDownUI);
	}
}

void GUI::Update()
{
	for (int i = 0; i < 3; i++)
	{
		hearthsSection[i] = { 28,0, 28, 24 };
	}
	for (int i = 0; i < _app->scene->playerSettings->playerLifes; i++)
	{
		// Full Hearths
		hearthsSection[i] = { 0,0, 28, 24 };
	}
	_app->ui->uiArray[scoreUI]->ChangeUI(_app->scene->playerSettings->playerScore);

	if (_app->scene->playerSettings->currentSkillCD > 0)
	{
		int coolDown = (int)_app->scene->playerSettings->currentSkillCD;
		if (coolDownUI == -1)
		{
			coolDownUI = _app->ui->CreateUI(coolDown, 148, 14, 0.5f, 2, 4);
		}
		else
		{
			_app->ui->uiArray[coolDownUI]->ChangeUI(coolDown);
		}
	
	}
	else if (_app->scene->playerSettings->currentSkillCD <= 0)
	{
		if (coolDownUI != -1 && _app->ui->uiArray[coolDownUI] != nullptr)
		{
			_app->ui->DestroyUI(coolDownUI);
			coolDownUI = -1;
		}
	}
}

void GUI::PostUpdate()
{
	_app->renderer->AddTextureRenderQueue(gui, { 0,0 }, { 0,0,0,0 }, 1, 2, 1, 0, SDL_FLIP_NONE, 0);

	for (int i = 0; i < 3; i++)
	{
		_app->renderer->AddTextureRenderQueue(hearths[i], { 35 * i + 16, 12 }, hearthsSection[i], 1, 2, 2, 0,SDL_FLIP_NONE, 0);
	}

	int skillCd = _app->scene->playerSettings->currentSkillCD > 0 ? 0 : 32;

	_app->renderer->AddTextureRenderQueue(skill[currentCharacter], { 144, 8 }, { skillCd,0,32,32 }, 1, 2, 2, 0, SDL_FLIP_NONE, 0);
}


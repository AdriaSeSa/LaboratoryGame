#include "StaticUI.h"

StaticUI::StaticUI(int character, Application* app)
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

	int highScore = _app->saveF.child("game_state").child("scene").child("player").attribute("highScore").as_int();

	// Score UI
	scoreUI = _app->ui->CreateUI(_app->scene->playerSettings->playerScore, 250, 8, 0.35f, 2,2);
	highScoreUI = _app->ui->CreateUI(highScore, 250, 24, 0.35f, 2, 2);
	minutesUI = _app->ui->CreateUI(0, 80, 16, 0.35f, 2, 2);
	secondsUI = _app->ui->CreateUI(0, 96, 16, 0.35f, 2, 2);

	seconds = minutes = 0;
}

StaticUI::~StaticUI()
{
	_app->ui->DestroyUI(scoreUI);
	_app->ui->DestroyUI(highScoreUI);
	_app->ui->DestroyUI(minutesUI);
	_app->ui->DestroyUI(secondsUI);
	if (coolDownUI != -1)
	{
		_app->ui->DestroyUI(coolDownUI);
	}
}

void StaticUI::Update()
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

	seconds += _app->globalTime.getDeltaTime()*10;
	printf("%f\n", _app->globalTime.getDeltaTime());
	if (seconds >= 60.0f)
	{
		minutes += 1;
		seconds = 0;
	}


	_app->ui->uiArray[secondsUI]->ChangeUI((int)seconds);
	_app->ui->uiArray[minutesUI]->ChangeUI((int)minutes);
}

void StaticUI::PostUpdate()
{
	_app->renderer->AddTextureRenderQueue(gui, { 0,0 }, { 0,0,0,0 }, 1, 2, 1, 0, SDL_FLIP_NONE, 0);

	for (int i = 0; i < 3; i++)
	{
		_app->renderer->AddTextureRenderQueue(hearths[i], { 16 * i + 16, 16 }, hearthsSection[i], 0.5f, 2, 2, 0,SDL_FLIP_NONE, 0);
	}

	int skillCd = _app->scene->playerSettings->currentSkillCD > 0 ? 0 : 32;

	_app->renderer->AddTextureRenderQueue(skill[currentCharacter], { 144, 8 }, { skillCd,0,32,32 }, 1, 2, 2, 0, SDL_FLIP_NONE, 0);
}


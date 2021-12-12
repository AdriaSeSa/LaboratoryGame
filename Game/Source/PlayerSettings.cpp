#include "PlayerSettings.h"
#include "Application.h"

PlayerSettings* PlayerSettings::mInstance = nullptr;

PlayerSettings::PlayerSettings()
{

}

PlayerSettings::~PlayerSettings()
{
}

PlayerSettings* PlayerSettings::Instance(Application* app)
{
	if (mInstance == nullptr)
	{
		mInstance = new PlayerSettings();
		mInstance->_app = app;
	}
	return mInstance;
}

void PlayerSettings::Release()
{
	if (mInstance != nullptr)
	{
		delete mInstance;
		mInstance = nullptr;
	}
}

void PlayerSettings::Reset()
{
	playerLifes = 3;
	playerScore = 0;
	reachedCheckPoint = false;
}

void PlayerSettings::AddScore(int num)
{
	int lastScore = playerScore;
	playerScore += num;

	if (playerScore % 1000 == 0 && playerScore != 0)
	{
		if (playerLifes < 3)
		{
			playerLifes++;
			_app->ui->CreateUI(1, 20, 8, 1, 2, 3, true, 90, { 0,-1 });
		}
		playerLifes < 3 ? playerLifes++ : playerLifes = playerLifes;
	}
}

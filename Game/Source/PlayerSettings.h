#pragma once

class Application;

class PlayerSettings
{
private:
	static PlayerSettings* mInstance;

	PlayerSettings();
	~PlayerSettings();

	Application* _app = nullptr;

public:

	static PlayerSettings* Instance(Application* app);

	static void Release();

	void Reset();

	void AddScore(int num);

public:

	int playerLifes = 0;

	int playerScore = 0;

	bool isLevelCompleted[2];

	bool isInit = false;

	bool reachedCheckPoint = false;

	float currentSkillCD = 0;
};



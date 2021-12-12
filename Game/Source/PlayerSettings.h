#pragma once

class Application;

class PlayerSettings
{
private:
	static PlayerSettings* mInstance;

	PlayerSettings();
	~PlayerSettings();

	Application* _app;

public:

	static PlayerSettings* Instance(Application* app);

	static void Release();

	void Reset();

	void AddScore(int num);

public:

	int playerLifes;

	int playerScore;

	bool isLevelCompleted[2];

	bool isInit = false;

	bool reachedCheckPoint = false;
};



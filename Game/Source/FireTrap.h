#pragma once
#include "GameObject.h"
#include "Animation.h"

class Player;

class FireTrap :public GameObject
{
public: 

	FireTrap(iPoint position, std::string name, std::string tag, Application* app);

	void Reset();

	void Update() override;

	void PostUpdate() override;

	void OnCollisionEnter(PhysBody* col) override;

	void OnCollisionExit(PhysBody* col) override;

	/// <summary>
	/// Fight!!!
	/// </summary>
	/// <param name="flashDuration">time in ms</param>
	/// <param name="fireDuration">time in ms</param>
	/// <returns>Si ya esta encencida, devolvera un false</returns>
	bool FireOn(int flashDuration = 0, int fireDuration = 0);

	bool HasPlayerIn();

private:
	iPoint startPos;

	int flashDuration = 0;
	int fireDuration = 0;

	bool activeteFire = false;
	Animation fireFlash;
	Animation fireOn;

	Player* player = nullptr;
};


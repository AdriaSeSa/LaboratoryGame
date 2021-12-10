#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "HitboxSensor.h"

class Player;

class FireTrap :public GameObject
{
public: 

	FireTrap(iPoint position, std::string name, std::string tag, Application* app, bool isController = true);

	~FireTrap();

	void Reset();

	void Update() override;

	void PostUpdate() override;

	void OnTriggerEnter(PhysBody* trigger, PhysBody* col) override;

	void OnTriggerExit(PhysBody* trigger, PhysBody* col) override;

	void CleanUp() override;

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

	HitboxSensor* hitBox;

	// Controlado por si mismo o por un script externo
	bool isController = true;

	int flashDuration = 0;
	int fireDuration = 0;

	bool activeteFire = false;
	Animation fireFlash;
	Animation fireOn;

	Player* player = nullptr;
};


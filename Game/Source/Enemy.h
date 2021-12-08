#pragma once
#include "GameObject.h"
#include "PathFinding.h"
#include "Player.h"

class Enemy : public GameObject
{
protected:

	int life;

	float speed;

	int score;

	Player* player = nullptr;

	bool movesDiagonally;

	bool isActive;

	PathFinding* pathFinding = nullptr;

public:
	Enemy(Player* player, std::string name, std::string tag, Application* app);

	virtual void Die();

	iPoint GetPathDirection(iPoint destination);

};
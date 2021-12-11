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

	Animation currentAnim;

	PathFinding* pathFinding = nullptr;

	// player offset when detect center of player
	iPoint playerOffset = { 5,6 };

protected:
	virtual void SetUpAnimations() = 0;

public:
	Enemy(Player* player, std::string name, std::string tag, Application* app);

	~Enemy();

	virtual void Die();

	iPoint GetPathDirection(iPoint destination);
};
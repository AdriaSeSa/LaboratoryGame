#pragma once
#include "GameObject.h"

class Player;

enum Directions
{
	
};

class Enemy : public GameObject
{
private:
	int life;
	
	float speed;
	
	int score;

	Player* player = nullptr;

public:
	Enemy(Player* player, std::string name, std::string tag, Application* app);

	iPoint GetPathDirection();
};


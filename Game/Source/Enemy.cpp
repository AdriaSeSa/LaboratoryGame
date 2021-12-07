#include "Enemy.h"

Enemy::Enemy(Player* player, std::string name, std::string tag, Application* app) : GameObject(name, tag, app)
{
	this->player = player;
}

iPoint Enemy::GetPathDirection()
{
	return iPoint();
}

#include "Enemy.h"

void Enemy::SetUpAnimations()
{
}

Enemy::Enemy(Player* player, std::string name, std::string tag, Application* app) : GameObject(name, tag, app)
{
	this->player = player;
	pathFinding = _app->map->pathFinding;
}

Enemy::~Enemy()
{
}

void Enemy::Die()
{
	if(player!=nullptr)
	{
		player->ResetJumpCount();
		player->Jump(false);
	}

	if (!isDie)
	{
		// Play Die Animation && Give score
		isDie = true;
		_app->audio->PlayFx(SFX::ENEMY_DIE);
		SetLinearVelocity(iPoint{ 0, 0});
		_app->scene->playerSettings->AddScore(score);
		_app->ui->CreateUI(score, GetPosition().x - 5, GetPosition().y, 0.3f, 2, 0, true, 90, { 0,-1 });
	}
}

iPoint Enemy::GetPathDirection(iPoint destination)
{
	iPoint destPos = _app->map->MapToWorld(_app->map->WorldToMap(destination));

	iPoint myPos = _app->map->MapToWorld(_app->map->WorldToMap(GetPosition()));

	// If is in destination tile
	if ((_app->map->WorldToMap(GetPosition()) == _app->map->WorldToMap(destination)))
	{
		return iPoint(0, 0);
	}

	if (pathFinding->CreatePath(_app->map->WorldToMap(GetPosition()), _app->map->WorldToMap(destination)) == -1) return iPoint(0,0);

	if(_app->debug->debugCollisionView)
	{
		const DynArray<iPoint>* path = pathFinding->GetLastPath();

		for (size_t i = 0; i < path->Count(); i++)
		{
			iPoint pos = _app->map->MapToWorld(path->At(i)->x, path->At(i)->y);

			_app->renderer->AddRectRenderQueue(SDL_Rect{ pos.x, pos.y, 16, 16 }, 255, 0, 0, 100, 3);
		}
	}

	iPoint direction;

	// Get first step on Pathfinding
	iPoint firstStep = pathFinding->GetStepFromLastPath(1) == NULL ? _app->map->WorldToMap(GetPosition()) : *pathFinding->GetStepFromLastPath(1);

	if (!movesDiagonally)
	{
		// Calculate direction for next step
		direction = firstStep - _app->map->WorldToMap(GetPosition());

		return direction;
	}

	// Get second step on Pathfinding
	iPoint secondStep = pathFinding->GetStepFromLastPath(2) == NULL ? firstStep : *pathFinding->GetStepFromLastPath(2);

	// Calculate direction to second Step
	direction = secondStep - _app->map->WorldToMap(GetPosition());

	// If direction is diagonal...
	if (abs(direction.x) == abs(direction.y))
	{
		iPoint directionX =  iPoint(direction.x, 0) + _app->map->WorldToMap(GetPosition());
		iPoint directionY = iPoint(0, direction.y) + _app->map->WorldToMap(GetPosition());

		// ... check if the sides are walkable
		if (_app->map->pathFinding->IsWalkable(directionX) && pathFinding->IsWalkable(directionY))
		{
			// Move diagonally
			return direction.Normalize();
		}
		else
		{
			// If they are not, return the next step and proceed normally
			direction = firstStep - _app->map->WorldToMap(GetPosition());
			return direction;
		}
	}
	else
	{
		// If there is not step 2, move to step 1
		direction = firstStep - _app->map->WorldToMap(GetPosition());
		return direction;
	}
}

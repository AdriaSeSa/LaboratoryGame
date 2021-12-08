#include "Enemy.h"

Enemy::Enemy(Player* player, std::string name, std::string tag, Application* app) : GameObject(name, tag, app)
{
	this->player = player;
	pathFinding = _app->map->pathFinding;
}

void Enemy::Die()
{
	// Play Die Animation && Give score && call CleanUp()
}

iPoint Enemy::GetPathDirection(iPoint destination)
{
	//printf("OriginX: %d OriginY: %d\tDestinationX: %d DestinationY: %d\n", _app->map->WorldToMap(GetPosition()).x, _app->map->WorldToMap(GetPosition()).y, 
	//_app->map->WorldToMap(destination).x, _app->map->WorldToMap(destination).y);

	if ((_app->map->WorldToMap(GetPosition()) == _app->map->WorldToMap(destination)))
	{
		printf("x = %d y = %d \n", _app->map->WorldToMap(GetPosition()).x, _app->map->WorldToMap(GetPosition()).y);
		return iPoint(0, 0);
	}
	
	if (pathFinding->CreatePath(_app->map->WorldToMap(GetPosition()), _app->map->WorldToMap(destination)) == -1) return iPoint(0,0);
	
	iPoint direction;
	if (!movesDiagonally)
	{
		// Get next step on the pathfinding
		const iPoint nextStep = *pathFinding->GetLastPath()->At(1);

		// Calculate direction for next step
		direction = nextStep - _app->map->WorldToMap(GetPosition());

		return direction;
	}

	// Get second step on Pathfinding
	iPoint secondStep = pathFinding->GetLastPath()->At(2) == NULL ? _app->map->WorldToMap(GetPosition()) : *pathFinding->GetLastPath()->At(2);

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
			// If they are, return the diagonal direction normalized
			return direction.Normalize();
		}
		else 
		{
			// If they are not, return the next step and proceed normally
			direction = *pathFinding->GetLastPath()->At(1) - _app->map->WorldToMap(GetPosition());
			return direction;
		}
	}
	else
	{
		// If it is not diagonal, return the next step and proceed normally
		direction = *pathFinding->GetLastPath()->At(1) - _app->map->WorldToMap(GetPosition());
		return direction;
	}

}

#include "BatEnemy.h"

BatEnemy::BatEnemy(iPoint pos, Player* player, std::string name, std::string tag, Application* app) : Enemy(player, name, tag, app)
{
	// Initialize enemy variables
	life = 1;
	score = 200;
	speed = 1.0f;
	movesDiagonally = true;
	initialPos = pos;

	// Create pBody
	this->pBody = _app->physics->CreateCircle(pos.x, pos.y, 6, this, true);
	pBody->body->SetGravityScale(0);

	// Create detecting sensor
	detectionSensor = new HitboxSensor(pos, detectionRadius, this, "batSensor", "BatSensor", _app);
	detectionSensor->hits[0] = "player";
}

void BatEnemy::Update()
{
	detectionSensor->SetPosition(GetPosition());

	// If we are not near the center of the tile, we dont update our pathfinding
	if (!_app->map->InTileCenter(GetPosition(), 2))return;

	// If is in destination tile
	if (_app->map->WorldToMap(GetPosition()) == _app->map->WorldToMap(player->GetPosition() + playerOffset))
	{
		// Don't move
		this->SetLinearVelocity(b2Vec2{0,0});
		return;
	}

	iPoint dir = GetPathDirection(player->GetPosition() + playerOffset);

	//Check if we are active and if there is a path to the player
	if (!isActive || dir == iPoint(0, 0))
	{
		if(_app->map->WorldToMap(GetPosition()) == _app->map->WorldToMap(initialPos))
		{
			iPoint offset = { 8,8 };
			SetPosition(_app->map->MapToWorld(_app->map->WorldToMap(GetPosition())) + offset);
		}
		else
		{
			// If we are not active, we return to our initialPos
			dir = GetPathDirection(initialPos);
		}
	}

	// Move to direction
	this->SetLinearVelocity(dir * speed);
}

void BatEnemy::OnCollisionEnter(PhysBody* col)
{
}

void BatEnemy::OnTriggerEnter(PhysBody* col)
{
	isActive = true;
}

void BatEnemy::OnTriggerExit(PhysBody* col)
{
	isActive = false;
}

void BatEnemy::CleanUp()
{
	RELEASE(detectionSensor);
}
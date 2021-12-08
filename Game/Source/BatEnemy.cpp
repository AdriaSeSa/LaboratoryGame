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
	if (!_app->map->InTileCenter(GetPosition()))return;

	iPoint dir = GetPathDirection(player->GetPosition());

	//iPoint dir = { 0,0 };

	//Check if we are active and if there is a path to the player
	if (!isActive || dir == iPoint(0, 0))
	{
		// If we are not active, we return to our initialPos
		dir = GetPathDirection(initialPos);
	}

	if (dir != iPoint(0, 0))
	{
		printf("x:%d y:%d\n", dir.x, dir.y);
	}
	// Move to direction
	this->SetLinearVelocity(dir);

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

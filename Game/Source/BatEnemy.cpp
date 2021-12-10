#include "BatEnemy.h"

BatEnemy::BatEnemy(iPoint pos, Player* player, std::string name, std::string tag, Application* app) : Enemy(player, name, tag, app)
{
	std::string texNames[5] = { "batIdle","batFlying","batCeilingIn","batCeilingOut","batHit"};

	for (int i = 0; i < 5; i++)
	{
		InitRenderObjectWithXml(texNames[i], i);
	}

	// Offset with axis Y
	renderObjects[BAT_IDLE].textureCenterY += 2;

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
	detectionSensor->hits[0] = "PlayerHitBox";

	// Animations Setup
	SetUpAnimations();

	ChangeState(BAT_IDLE);
}

void BatEnemy::PreUpdate()
{
	if(batState==BAT_HIT && currentAnim.HasFinished())
	{
		pendingToDelete = true;
	}
}

void BatEnemy::Update()
{
	if (isDie) return;

	detectionSensor->SetPosition(GetPosition());

	// If we are not near the center of the tile, we dont update our pathfinding
	if (!_app->map->InTileCenter(GetPosition(), 4))return;

	// If is in destination tile
	if (_app->map->WorldToMap(GetPosition()) == _app->map->WorldToMap(player->GetPosition() + playerOffset))
	{
		// Don't move
		this->SetLinearVelocity(b2Vec2{ 0, 0 });
		return;
	}

	iPoint dir = GetPathDirection(player->GetPosition() + playerOffset);

	//Check if we are active and if there is a path to the player
	if (!isActive || dir == iPoint( 0, 0))
	{
		if(_app->map->WorldToMap(GetPosition()) == _app->map->WorldToMap(initialPos))
		{
			if (batState == BAT_FLYING)
			{
				ChangeState(BAT_CEILINGIN);
			}
			iPoint offset = { 8,8 };
			SetPosition(_app->map->MapToWorld(_app->map->WorldToMap(GetPosition())) + offset);
		}
		else
		{
			// If we are not active, we return to our initialPos
			dir = GetPathDirection(initialPos);
		}
	}
	else if(isActive)
	{
 		if (batState == BAT_IDLE)
		{
			ChangeState(BAT_CEILINGOUT);
		}
		else if (batState == BAT_CEILINGIN)
		{
			ChangeState(BAT_FLYING);
		}
	}

	if(batState == BAT_CEILINGOUT || batState == BAT_IDLE)
	{
		// Don't move when is CeilingOut
		this->SetLinearVelocity(b2Vec2{ 0, 0 });
	}
	else
	{
		// Move to direction
 		this->SetLinearVelocity(dir * speed);
	}
}

void BatEnemy::PostUpdate()
{
	currentAnim.Update();

	switch (batState)
	{
	case BAT_IDLE:
		//idleAnim.Update();
		break;
	case BAT_FLYING:
		//flyingAnim.Update();
		break;
	case BAT_CEILINGIN:
		if (currentAnim.HasFinished()) ChangeState(BAT_STATE::BAT_IDLE);
		break;
	case BAT_CEILINGOUT:
		if (currentAnim.HasFinished()) ChangeState(BAT_STATE::BAT_FLYING);
		break;
	case BAT_HIT:
		if (currentAnim.HasFinished()) renderObjects[batState].draw = false;
	}
	renderObjects[batState].section = currentAnim.GetCurrentFrame();

	if(batState == BAT_FLYING)
	renderObjects[batState].flip = GetLinearVelocity().x > 0 ? SDL_RendererFlip::SDL_FLIP_HORIZONTAL : SDL_RendererFlip::SDL_FLIP_NONE;

	GameObject::PostUpdate();
}

void BatEnemy::OnCollisionEnter(PhysBody* col)
{
	if(col->gameObject->CompareTag("GroundSensor"))
	{
		if(!isDie)
		{
			ChangeState(BAT_HIT);
			Die();
		}
	}
}

void BatEnemy::OnTriggerEnter(PhysBody* trigger, PhysBody* col)
{
	isActive = true;
}

void BatEnemy::OnTriggerExit(PhysBody* trigger, PhysBody* col)
{
	isActive = false;
}

void BatEnemy::CleanUp()
{
	RELEASE(detectionSensor);
}

void BatEnemy::SetUpAnimations()
{
	for (int i = 0; i < 12; i++)
	{
		anims[BAT_IDLE].PushBack({ 46 * i, 0, 46, 30});
	}

	for (int i = 0; i < 7; i++)
	{
		anims[BAT_FLYING].PushBack({ 46 * i, 0, 46, 30});
	}

	for (int i = 0; i < 7; i++)
	{
		anims[BAT_CEILINGIN].PushBack({ 46 * i, 0, 46, 30});
	}

	for (int i = 0; i < 7; i++)
	{
		anims[BAT_CEILINGOUT].PushBack({ 46 * i, 0, 46, 30});
	}

	for (int i = 0; i < 5; i++)
	{
		anims[BAT_HIT].PushBack({ 46 * i, 0, 46, 30});
	}

	anims[BAT_HIT].hasIdle = false;
	anims[BAT_HIT].speed = 0.3f;
	anims[BAT_HIT].loop = false;

	anims[BAT_CEILINGIN].hasIdle = false;
	anims[BAT_CEILINGIN].speed = 0.2f;
	anims[BAT_CEILINGIN].loop = false;

	anims[BAT_CEILINGOUT].hasIdle = false;
	anims[BAT_CEILINGOUT].speed = 0.2f;
	anims[BAT_CEILINGOUT].loop = false;

	anims[BAT_IDLE].hasIdle = false;
	anims[BAT_IDLE].speed = 0.3f;

	anims[BAT_FLYING].hasIdle = false;
	anims[BAT_FLYING].speed = 0.3f;
}

void BatEnemy::ChangeState(BAT_STATE state)
{
	for (int i = 0; i < 5; i++)
	{
		renderObjects[i].draw = false;
	}

	// Change Texture
	renderObjects[state].draw = true;

	// Change Animation
	currentAnim = anims[state];
	currentAnim.Reset();

	batState = state;
}

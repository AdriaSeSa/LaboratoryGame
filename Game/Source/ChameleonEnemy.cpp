#include "ChameleonEnemy.h"

ChameleonEnemy::ChameleonEnemy(iPoint pos, Player* player, std::string name, std::string tag, Application* app) : Enemy(player, name, tag, app)
{
	std::string texNames[4] = { "chameleonIdle","chameleonRun","chameleonAttack","chameleonHit"};

	for (int i = 0; i < 4; i++)
	{
		InitRenderObjectWithXml(texNames[i], i);

		// Offset with axis X
		renderObjects[i].textureCenterX -= 10;
		// Offset with axis Y
		renderObjects[i].textureCenterY += 2;
	}

	// Initialize enemy variables
	life = 2;
	score = 300;
	speed = 1.0f;
	movesDiagonally = false;

	// Create pBody
	pBody = _app->physics->CreateCircle(pos.x, pos.y, 7, this, true);
	pBody->SetSensor(false);
	pBody->body->SetFixedRotation(true);

	// Must not collision with player
	b2Filter physicFilter;
	physicFilter.groupIndex = -1;
	pBody->body->GetFixtureList()->SetFilterData(physicFilter);

	// Create detecting sensor
	detectionSensor = new HitboxSensor(pos + detectionOffset, 400, 60, this, "chameleonSensor", "ChameleonSensor", _app);
	detectionSensor->hits[0] = "PlayerHitBox";

	// Create attack sensor
	attack = new HitboxSensor(pos, 25, 2, this, "chameleonAttack", "ChameleonAttack", _app);
	attack->hits[0] = "PlayerHitBox";
	attack->enable = false;

	// Animations Setup
	SetUpAnimations();

	// Set initial state
	ChangeState(CHAMELEON_IDLE);
}

ChameleonEnemy::ChameleonEnemy(iPoint pos, Player* player, int ID, int lifes, std::string name, std::string tag, Application* app) : Enemy(player, name, tag, app)
{
	std::string texNames[4] = { "chameleonIdle","chameleonRun","chameleonAttack","chameleonHit" };

	for (int i = 0; i < 4; i++)
	{
		InitRenderObjectWithXml(texNames[i], i);

		// Offset with axis X
		renderObjects[i].textureCenterX -= 10;
		// Offset with axis Y
		renderObjects[i].textureCenterY += 2;
	}

	// Initialize enemy variables
	life = lifes;
	this->ID = ID;
	score = 300;
	speed = 1.0f;
	movesDiagonally = false;

	// Create pBody
	pBody = _app->physics->CreateCircle(pos.x, pos.y, 7, this, true);
	pBody->SetSensor(false);
	pBody->body->SetFixedRotation(true);

	// Must not collision with player
	b2Filter physicFilter;
	physicFilter.groupIndex = -1;
	pBody->body->GetFixtureList()->SetFilterData(physicFilter);

	// Create detecting sensor
	detectionSensor = new HitboxSensor(pos + detectionOffset, 400, 60, this, "chameleonSensor", "ChameleonSensor", _app);
	detectionSensor->hits[0] = "PlayerHitBox";

	// Create attack sensor
	attack = new HitboxSensor(pos, 25, 2, this, "chameleonAttack", "ChameleonAttack", _app);
	attack->hits[0] = "PlayerHitBox";
	attack->enable = false;

	// Animations Setup
	SetUpAnimations();

	// Set initial state
	ChangeState(CHAMELEON_IDLE);
}

void ChameleonEnemy::Update()
{
	if (isDie || chameleonState == CHAMELEON_ATTACK  || chameleonState == CHAMELEON_HIT) return;

	// Update detectionSensor position
	detectionSensor->SetPosition(GetPosition() + detectionOffset);

	if (chameleonMode == CHAMELEON_GUARD_MODE)
	{
		// No hace nada
	}
	else if (chameleonMode == CHAMELEON_CHASE_MODE)
	{
		// If we are not near the center of the tile, we dont update our pathfinding
		if (!_app->map->InTileCenter(GetPosition(), 4) && GetLinearVelocity().x != 0) return;

		iPoint dir = GetPathDirection(player->GetPosition() + playerOffset);

		if (chameleonState != CHAMELEON_RUN)
		{
			ChangeState(CHAMELEON_RUN);
		}

		if (dir.y == 0)
		{
			this->SetLinearVelocity(dir * speed);
		}
		else if (dir.y < 0)
		{
			chameleonMode = CHAMELEON_PATROL_MODE;
		}
	}
	if (chameleonMode == CHAMELEON_PATROL_MODE)
	{
		b2Vec2 vel = GetLinearVelocity();

		if (vel.x != 0 && _app->map->InTileCenter(GetPosition(), 4))
		{
			iPoint mapPos = _app->map->WorldToMap(GetPosition());
			iPoint mapPosDiagonal = mapPos;

			mapPos.x += vel.x > 0 ? 1 : -1;
			mapPosDiagonal.x = mapPos.x;
			mapPosDiagonal.y += 1;

			// Si el siguiente tile esta vacio
			if (!_app->map->pathFinding->IsWalkable(mapPos) || _app->map->pathFinding->IsWalkable(mapPosDiagonal))
			{			
				// Cambia sentido
				int dir = vel.x > 0 ? -1 : 1;
				SetLinearVelocity(b2Vec2{ dir * speed, GetLinearVelocity().y});
			}
		}
		else if(vel.x == 0)
		{
			int randDir = rand() % 2;
			randDir = randDir == 0 ? -1 : randDir;
			
			SetLinearVelocity(b2Vec2{ randDir * speed , GetLinearVelocity().y});
		}

		if (chameleonState != CHAMELEON_RUN)
		{
			ChangeState(CHAMELEON_RUN);
		}
	}
}

void ChameleonEnemy::PreUpdate()
{
	// Is is die, change pendingToDelete true
	if (life <=0 && chameleonState == CHAMELEON_HIT && currentAnim.HasFinished() || isDie && chameleonState == CHAMELEON_IDLE)
	{
		pendingToDelete = true;
	}
}

void ChameleonEnemy::PostUpdate()
{
	CheckFlip();

	// Update Animation
	currentAnim.Update();
 	renderObjects[chameleonState].section = currentAnim.GetCurrentFrame();

	switch (chameleonState)
	{
	case CHAMELEON_IDLE:
		break;
	case CHAMELEON_RUN:
		break;
	case CHAMELEON_ATTACK:
		Attack();
		break;
	case CHAMELEON_HIT:
		if (currentAnim.HasFinished())
		{
			renderObjects[chameleonState].draw = false;
			if (life > 0 && !isDie)
			{
				if (GetLinearVelocity().x != 0) ChangeState(CHAMELEON_RUN);
				else ChangeState(CHAMELEON_IDLE);
			}
			else
			{
				pendingToDelete = true;
			}
		}
		break;	
	}

	GameObject::PostUpdate();
}

void ChameleonEnemy::OnCollisionEnter(PhysBody* col)
{
	if (isDie) return;

	if (col->gameObject->CompareTag("GroundSensor") && col->gameObject->GetLinearVelocity().y > 0)
	{
		if (!isDie)
		{
			attack->enable = false;

			ChangeState(CHAMELEON_HIT);

			if(--life == 0)
			{
				pBody->body->SetGravityScale(0);
				pBody->SetSensor(true);
				Die();
			}
			else if(player != nullptr)
			{
				if (life == 1)
				{
 					speed = 2;
					anims[CHAMELEON_RUN].speed = 0.6f;
					int dir = GetLinearVelocity().x > 0 ? 1 : -1;
					SetLinearVelocity(b2Vec2{ dir * speed, GetLinearVelocity().y});
					ChangeSecondTexture();
				}
				player->ResetJumpCount();
				player->Jump();
			}
		}
	}
}

void ChameleonEnemy::OnTriggerEnter(PhysBody* trigger, PhysBody* col)
{
	if (isDie) return;

	if (trigger->gameObject->CompareTag("ChameleonSensor"))
	{
		chameleonMode = CHAMELEON_CHASE_MODE;
	}
	// Attack!!!
	else if (trigger->gameObject->CompareTag("ChameleonAttack") && chameleonState != CHAMELEON_ATTACK)
	{
		// Don't move when attack
		this->SetLinearVelocity(b2Vec2{ 0, GetLinearVelocity().y});
		ChangeState(CHAMELEON_ATTACK);
	}
}

void ChameleonEnemy::OnTriggerExit(PhysBody* trigger, PhysBody* col)
{
	if (trigger->gameObject->CompareTag("ChameleonSensor"))
	{
		chameleonMode = CHAMELEON_PATROL_MODE;
	}
}

void ChameleonEnemy::CleanUp()
{
	RELEASE(detectionSensor);
	RELEASE(attack);
}

void ChameleonEnemy::SetUpAnimations()
{
	for (int i = 0; i < 13; i++)
	{
		anims[CHAMELEON_IDLE].PushBack({ 84 * i, 0, 84, 38 });
	}

	for (int i = 0; i < 8; i++)
	{
		anims[CHAMELEON_RUN].PushBack({ 84 * i, 0, 84, 38 });
	}

	for (int i = 0; i < 10; i++)
	{
		anims[CHAMELEON_ATTACK].PushBack({ 84 * i, 0, 84, 38 });
	}

	for (int i = 0; i < 5; i++)
	{
		anims[CHAMELEON_HIT].PushBack({ 84 * i, 0, 84, 38 });
	}

	anims[CHAMELEON_IDLE].hasIdle = false;
	anims[CHAMELEON_IDLE].speed = 0.3f;

	anims[CHAMELEON_RUN].hasIdle = false;
	anims[CHAMELEON_RUN].speed = 0.2f;

	anims[CHAMELEON_ATTACK].hasIdle = false;
	anims[CHAMELEON_ATTACK].speed = 0.2f;
	anims[CHAMELEON_ATTACK].loop = false;

	anims[CHAMELEON_HIT].hasIdle = false;
	anims[CHAMELEON_HIT].speed = 0.3f;
	anims[CHAMELEON_HIT].loop = false;
}

void ChameleonEnemy::CheckFlip()
{
	flip = GetLinearVelocity().x > 0 ? true : GetLinearVelocity().x < 0 ? false : flip;

	// Update Attack position
	iPoint attackPos = GetPosition();

	if (flip)
	{
		attackPos.x += 16;
		renderObjects[chameleonState].flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
		renderObjects[chameleonState].textureCenterX = (renderObjects[chameleonState].destRect.w / 2) * (renderObjects[chameleonState].scale) - 10;
	}
	else
	{
		attackPos.x -= 16;
		renderObjects[chameleonState].flip = SDL_RendererFlip::SDL_FLIP_NONE;
		renderObjects[chameleonState].textureCenterX = (renderObjects[chameleonState].destRect.w / 2) * (renderObjects[chameleonState].scale) + 10;
	}

	attack->SetPosition(attackPos);
}

void ChameleonEnemy::Attack()
{
	if (IN_RANGE(currentAnim.getCurrentFrameI(), 6, 7))
	{
		attack->enable = true;
	}
	else
	{
		attack->enable = false;
	}

	if (currentAnim.HasFinished())
	{
		if (GetLinearVelocity().x != 0) ChangeState(CHAMELEON_RUN);
		else ChangeState(CHAMELEON_IDLE);
	}
}

void ChameleonEnemy::ChangeSecondTexture()
{
	std::string texNames[4] = { "chameleonIdle2","chameleonRun2","chameleonAttack2","chameleonHit2" };

	for (int i = 0; i < 4; i++)
	{
		InitRenderObjectWithXml(texNames[i], i);

		if (flip)
		{
			renderObjects[i].flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
			renderObjects[i].textureCenterX = (renderObjects[i].destRect.w / 2) * (renderObjects[i].scale) - 10;
		}
		else
		{
			renderObjects[i].flip = SDL_RendererFlip::SDL_FLIP_NONE;
			renderObjects[i].textureCenterX = (renderObjects[i].destRect.w / 2) * (renderObjects[i].scale) + 10;
		}

		// Offset with axis Y
		renderObjects[i].textureCenterY += 2;
	}
}

void ChameleonEnemy::ChangeState(CHAMELEON_STATE state)
{
	for (int i = 0; i < 4; i++)
	{
		renderObjects[i].draw = false;
	}

	// Change Texture
	renderObjects[state].draw = true;

	// Change Animation
	currentAnim = anims[state];
	currentAnim.Reset();

	chameleonState = state;
}
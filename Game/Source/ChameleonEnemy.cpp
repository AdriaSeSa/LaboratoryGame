#include "ChameleonEnemy.h"

ChameleonEnemy::ChameleonEnemy(iPoint pos, Player* player, std::string name, std::string tag, Application* app) : Enemy(player, name, tag, app)
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

	// Test code
	flip = false;

	// Initialize enemy variables
	life = 2;
	score = 300;
	speed = 1.0f;
	movesDiagonally = false;

	// Create pBody
	pBody = _app->physics->CreateCircle(pos.x, pos.y, 7, this, true);
	pBody->SetSensor(false);

	// Must not collision with player
	b2Filter physicFilter;
	physicFilter.groupIndex = -1;
	pBody->body->GetFixtureList()->SetFilterData(physicFilter);

	// Create detecting sensor
	detectionSensor = new HitboxSensor(pos, 20, 10, this, "chameleonSensor", "ChameleonSensor", _app);
	detectionSensor->hits[0] = "Player";

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
	if (isDie) return;

	detectionSensor->SetPosition(GetPosition());

	CheckFlip();
}

void ChameleonEnemy::PreUpdate()
{
	if (life <=0 && chameleonState == CHAMELEON_HIT && currentAnim.HasFinished())
	{
		pendingToDelete = true;
	}
}

void ChameleonEnemy::PostUpdate()
{
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
			if(life>0) ChangeState(CHAMELEON_IDLE);		
		}
		break;	
	}

	GameObject::PostUpdate();
}

void ChameleonEnemy::OnCollisionEnter(PhysBody* col)
{
	if (col->gameObject->CompareTag("GroundSensor"))
	{
		if (!isDie)
		{
			ChangeState(CHAMELEON_HIT);

			if(--life == 0)
			{
				pBody->body->SetGravityScale(0);
				pBody->SetSensor(true);
				Die();
			}
			else if(player != nullptr)
			{
				player->ResetJumpCount();
				player->Jump();
			}
		}
	}
}

void ChameleonEnemy::OnTriggerEnter(PhysBody* trigger, PhysBody* col)
{
	if (trigger->gameObject->CompareTag("ChameleonAttack") && chameleonState != CHAMELEON_ATTACK)
	{
		ChangeState(CHAMELEON_ATTACK);
	}
}

void ChameleonEnemy::OnTriggerExit(PhysBody* trigger, PhysBody* col)
{
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

	if (currentAnim.HasFinished()) ChangeState(CHAMELEON_IDLE);
}

void ChameleonEnemy::ChangeState(CHAMELEON_STATE state)
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

	chameleonState = state;
}
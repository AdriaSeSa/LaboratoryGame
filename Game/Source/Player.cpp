#include "Player.h"

Player::Player(iPoint pos, std::string name, std::string tag, Application* app) : GameObject(name, tag, app)
{
	std::string texNames[6] = { "virtualGuyIdle","virtualGuyRun","virtualGuyJump","virtualGuyFall","virtualGuyDoubleJump", "characterAppearing"};

	for (int i = 0; i < 6; i++)
	{
		InitRenderObjectWithXml(texNames[i], i);
	}

	//Phys Body
	//pBody = _app->physics->CreateRectangle(pos, 11, 16, this);

	// Puede ser que soluciona que el player pega en la pared
	pBody = _app->physics->CreateCircle(pos.x, pos.y, 6, this);

	//b2CircleShape shape;
	//shape.m_radius = PIXELS_TO_METER(6);
	//b2FixtureDef fixture;
	//fixture.shape = &shape;
	//fixture.friction = 0;
	//fixture.density = 0;
	//pBody->body->CreateFixture(&fixture);

	pBody->body->SetFixedRotation(true);

	pBody->body->SetBullet(true);

	pBody->body->GetFixtureList()->SetFriction(0);

	pBody->body->SetGravityScale(gravityScale);

	appliedFallForce = false;

	openPlatformSensor = new GroundSensor(GetPosition() + platformSensorOffset, "PlayerPSensor", "PlatformSensor", _app, 10, 6);

	closePlatformSensor = new GroundSensor(GetPosition() + iPoint{0, -2}, "PlayerPSensor", "PlatformSensorClose", _app, 18, 20);

	groundSensor = new GroundSensor(GetPosition() + groundSensorOffset, "PlayerGSensor", "GroundSensor", _app, 8, 4);

	hitBoxSensor = new HitboxSensor(GetPosition() + iPoint(-2,0),6, 8,this, "PlayerHitBox", "PlayerHitBox", _app);

	b2Filter physicFilter;
	physicFilter.groupIndex = -1;

	pBody->body->GetFixtureList()->SetFilterData(physicFilter);
	//pBody->body->GetFixtureList()->GetNext()->SetFilterData(physicFilter);
	groundSensor->pBody->body->GetFixtureList()->SetFilterData(physicFilter);
	hitBoxSensor->pBody->body->GetFixtureList()->SetFilterData(physicFilter);

	hitBoxSensor->hits[0] = "saw";
	hitBoxSensor->hits[1] = "spike";
	hitBoxSensor->hits[2] = "fireTramp";

	// Animations
	for (int i = 0; i < 11; i++)
	{
		idle.PushBack({ 32 * i, 0, 32, 32 });
	}

	for (int i = 0; i < 12; i++)
	{
		run.PushBack({ 32 * i, 0, 32, 32 });
	}

	for (int i = 0; i < 6; i++)
	{
		doubleJump.PushBack({ 32 * i, 0, 32, 32 });
	}

	for (int i = 0; i < 7; i++)
	{
		appearing.PushBack({ 96 * i,0,96,96 });
	}

	appearing.hasIdle = false;
	appearing.speed = 0.3f;
	appearing.loop = false;

	idle.hasIdle = false;
	idle.speed = 0.3f;

	run.hasIdle = false;
	run.speed = 0.3f;

	doubleJump.hasIdle = false;
	doubleJump.speed = 0.3f;
}

Player::~Player()
{
}

void Player::PreUpdate()
{
	groundSensor->SetPosition(GetPosition() + groundSensorOffset);
	hitBoxSensor->SetPosition(GetPosition() + iPoint(1, 0));

	openPlatformSensor->SetPosition(GetPosition() + platformSensorOffset);
	closePlatformSensor->SetPosition(GetPosition() + iPoint{ 0, -2});

	isFalling = pBody->body->GetLinearVelocity().y > fallDetection;

	if (groundSensor->isOnGround && jumpCount < 2)
	{
		//printf("inGround");
		jumpCount = 2;
	}

	if (appliedFallForce && !isFalling)
	{
		appliedFallForce = false;
	}

	if (isFalling && !appliedFallForce)
	{
		pBody->body->ApplyLinearImpulse({ 0,0.8f }, { 0,0, }, true);
		appliedFallForce = true;
	}
}

void Player::Update()
{
	if (GetPosition().y > 620)
	{
		lifes = 0;
		Die();
	}

	// ChangeGravity
	//if (_app->input->GetKey(SDL_SCANCODE_G) == KEY_UP)
	//{
	//	gravityScale = -gravityScale;
	//	pBody->body->SetGravityScale(gravityScale);
	//	pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, (float)gravityScale/2});
	//}

	// godMod
	if (_app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godMod = !godMod;
	}

	if (_app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		//pBody->body->SetLinearVelocity({ 0,speed });
		pBody->body->ApplyLinearImpulse({ 0, speed/2 }, { 0,0, }, true);
	}

	// Move
	if (_app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		pBody->body->SetLinearVelocity({ speed,pBody->body->GetLinearVelocity().y });
	}
	else if(_app->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		pBody->body->SetLinearVelocity({ 0,pBody->body->GetLinearVelocity().y });
	}
	if (_app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		pBody->body->SetLinearVelocity({ -speed,pBody->body->GetLinearVelocity().y });
	}
	else if (_app->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		pBody->body->SetLinearVelocity({ 0,pBody->body->GetLinearVelocity().y  });
	}

	// Jump
	if (_app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		if(godMod)
		{
			pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, -jumpForce });
		}
		else if (jumpCount != 0 && !jumpBlock)
		{
			groundSensor->SetOffGround();

			pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, -jumpForce });

			jumpCount--;
		}
	}

	UpdatePlayerState();
}

void Player::UpdatePlayerState()
{
	if (isFalling)
	{
		playerCurrentState = FALL;
		return;
	}

	if (pBody->body->GetLinearVelocity().y < -fallDetection)
	{
		if (jumpCount != 0)
		{
			playerCurrentState = JUMP;
			return;
		}
		else
		{
			playerCurrentState = DOUBLE_JUMP;
			doubleJump.Update();
			return;
		}
	}
	if (pBody->body->GetLinearVelocity().x > 0 || pBody->body->GetLinearVelocity().x < 0)
	{
		playerCurrentState = RUN;
		run.Update();
		return;
	}

	playerCurrentState = IDLE;
	idle.Update();
}

void Player::OnTriggerEnter(PhysBody* col)
{
	if (col->gameObject->name == "spike" || col->gameObject->name == "saw")
	{
		Die();
	}
}

void Player::Start()
{
	PlayerAppear();
}

void Player::PostUpdate()
{
	if (!isAppear)
	{
		appearing.Update();

		renderObjects[5].section = appearing.GetCurrentFrame();
		renderObjects[5].destRect.x = GetDrawPosition().x;
		renderObjects[5].destRect.y = GetDrawPosition().y;

		_app->renderer->AddTextureRenderQueue(renderObjects[5].texture, { renderObjects[5].destRect.x - 16,renderObjects[5].destRect.y - 16 },
			renderObjects[5].section, renderObjects[5].scale, renderObjects[5].layer, renderObjects[5].orderInLayer,
			renderObjects[5].rotation, renderObjects[5].flip, renderObjects[5].speedRegardCamera);

		if (appearing.HasFinished())
		{
			isAppear = true;
			_app->physics->Pause(false);
		}

		return;
	}

	int i = 0;

	switch (playerCurrentState)
	{
	case IDLE:
		i = 0;
		break;
	case RUN:
		i = 1;
		break;

	case JUMP:
		i = 2;
		break;

	case FALL:
		i = 3;
		break;
	case DOUBLE_JUMP:
		i = 4;
		break;
	}

	if (renderObjects[i].texture != nullptr)
	{
		renderObjects[i].destRect.x = GetDrawPosition().x;
		renderObjects[i].destRect.y = GetDrawPosition().y;

		isLookingLeft = pBody->body->GetLinearVelocity().x < 0 ? true : pBody->body->GetLinearVelocity().x > 0 ? false : isLookingLeft;

		renderObjects[i].flip = isLookingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

		if (i == 0)
		{
			_app->renderer->AddTextureRenderQueue(renderObjects[i].texture, { renderObjects[i].destRect.x,renderObjects[i].destRect.y },
				idle.GetCurrentFrame(), renderObjects[i].scale, renderObjects[i].layer, renderObjects[i].orderInLayer,
				renderObjects[i].rotation, renderObjects[i].flip, renderObjects[i].speedRegardCamera);
			return;
		}

		if (i == 1)
		{
			_app->renderer->AddTextureRenderQueue(renderObjects[i].texture, { renderObjects[i].destRect.x,renderObjects[i].destRect.y },
				run.GetCurrentFrame(), renderObjects[i].scale, renderObjects[i].layer, renderObjects[i].orderInLayer,
				renderObjects[i].rotation, renderObjects[i].flip, renderObjects[i].speedRegardCamera);
			return;
		}

		if (i == 4)
		{
			_app->renderer->AddTextureRenderQueue(renderObjects[i].texture, { renderObjects[i].destRect.x,renderObjects[i].destRect.y },
				doubleJump.GetCurrentFrame(), renderObjects[i].scale, renderObjects[i].layer, renderObjects[i].orderInLayer,
				renderObjects[i].rotation, renderObjects[i].flip, renderObjects[i].speedRegardCamera);
			return;
		}

		_app->renderer->AddTextureRenderQueue(renderObjects[i].texture, { renderObjects[i].destRect.x,renderObjects[i].destRect.y },
			renderObjects[i].section, renderObjects[i].scale, renderObjects[i].layer, renderObjects[i].orderInLayer,
			renderObjects[i].rotation, renderObjects[i].flip, renderObjects[i].speedRegardCamera);
	}
}

void Player::OnCollisionEnter(PhysBody* col)
{
	//printf_s("PlayerCol");
	if (col->gameObject->CompareTag("MobilePlatform"))
	{
		fallDetection = 10;

		if (col->gameObject->name == "specialPlatform")
		{
			jumpBlock = true;
		}
	}
}

void Player::OnCollisionExit(PhysBody* col)
{
	//printf_s("PlayerColExit");
	if (col->gameObject->CompareTag("MobilePlatform"))
	{
		fallDetection = 0.1f;

		if (col->gameObject->name == "specialPlatform")
		{
			jumpBlock = false;
		}
	}
}

void Player::CleanUp()
{
	if (groundSensor != nullptr)
	{
		delete groundSensor;
		groundSensor = nullptr;
	}
	if (hitBoxSensor != nullptr)
	{
		delete hitBoxSensor;
		hitBoxSensor = nullptr;
	}
	if (openPlatformSensor != nullptr)
	{
		delete openPlatformSensor;
		openPlatformSensor = nullptr;
	}
	if (closePlatformSensor != nullptr)
	{
		delete closePlatformSensor;
		closePlatformSensor = nullptr;
	}
}

void Player::PlayerAppear()
{
	isAppear = false;
	_app->physics->Pause(true);
	appearing.Reset();
}

void Player::Die()
{
	if(!godMod)
	{
		isDead = true;
	}
}

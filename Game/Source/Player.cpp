#include "Player.h"
#include "Scene.h"

Player::Player(iPoint pos, std::string name, std::string tag, Application* app) : GameObject(name, tag, app)
{
	std::string texNames[6] = { "virtualGuyIdle","virtualGuyRun","virtualGuyJump","virtualGuyFall","virtualGuyDoubleJump", "characterAppearing"};

	usingSkill = false;

	skillCoolDown = 0;

	for (int i = 0; i < 6; i++)
	{
		InitRenderObjectWithXml(texNames[i], i);
	}

	int playerChain[8] =
	{
		0,0,
		9,0,
		9,12,
		0,12
	};

	//Phys Body
	pBody = _app->physics->CreateChainObj(pos.x, pos.y, playerChain, 8, true, this);

	pBody->body->SetFixedRotation(true);

	pBody->body->SetBullet(true);

	pBody->body->GetFixtureList()->SetFriction(0);

	pBody->body->SetGravityScale(gravityScale);

	// Sensors
	openPlatformSensor = new GroundSensor(GetPosition() + platformSensorOffset, "PlayerPSensor", "PlatformSensor", _app, 10, 6, this);

	closePlatformSensor = new GroundSensor(GetPosition() + iPoint{ 5, 6 }, "PlayerPSensor", "PlatformSensorClose", _app, 16, 20, this);

	groundSensor = new GroundSensor(GetPosition() + groundSensorOffset, "PlayerGSensor", "GroundSensor", _app, 8, 2, this);

	hitBoxSensor = new HitboxSensor(GetPosition() + iPoint(3,6), 7, 8,this, "PlayerHitBox", "PlayerHitBox", _app);

	hitBoxSensor->pBody->body->SetType(b2BodyType::b2_dynamicBody);

	// Set collisions filter
	b2Filter physicFilter;
	physicFilter.groupIndex = -1;

	pBody->body->GetFixtureList()->SetFilterData(physicFilter);
	//groundSensor->pBody->body->GetFixtureList()->SetFilterData(physicFilter);
	//hitBoxSensor->pBody->body->GetFixtureList()->SetFilterData(physicFilter);

	hitBoxSensor->hits[0] = "saw";
	hitBoxSensor->hits[1] = "spike";
	hitBoxSensor->hits[2] = "bat";
	hitBoxSensor->hits[3] = "chameleon";
	hitBoxSensor->hits[4] = "chameleonAttack";

	// Animations Setup
	SetUpAnimations();
}

Player::~Player()
{
}

void Player::Start()
{
	PlayerAppear();
}

void Player::SetUpAnimations()
{
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
	appearing.speed = 0.2f;
	appearing.loop = false;

	idle.hasIdle = false;
	idle.speed = 0.3f;

	run.hasIdle = false;
	run.speed = 0.3f;

	doubleJump.hasIdle = false;
	doubleJump.speed = 0.3f;
}

void Player::PlayerAppear()
{
	SetLinearVelocity(iPoint(0, 0));
	isAppear = false;
	_app->physics->Pause(true);
	appearing.Reset();
}

void Player::PreUpdate()
{
	// Update sensors position
	groundSensor->SetPosition(GetPosition() + groundSensorOffset);
	hitBoxSensor->SetPosition(GetPosition() + iPoint(3, 6));

	// Sensor for open or close platform collision
	openPlatformSensor->SetPosition(GetPosition() + platformSensorOffset);
	closePlatformSensor->SetPosition(GetPosition() + iPoint{ 5, 4});

	// Update falling, jumpcount and appliedDallForce variables
	isFalling = pBody->body->GetLinearVelocity().y > fallDetection;

	if (groundSensor->isOnGround && jumpCount < 2)
	{
		ResetJumpCount();
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

	if(parent != nullptr)
	{
		relativeVelocity_X = parent->gameObject->GetLinearVelocity().x;
	}

	if (groundSensor->isOnGround) usingSkill = false;
}

void Player::Update()
{
	if (skillCoolDown > 0)
	{
		_app->globalTime.Update();
		skillCoolDown -= _app->globalTime.getDeltaTime()*10;
	}
	
	_app->scene->playerSettings->currentSkillCD = skillCoolDown;

	// ChangeGravity
	//if (_app->input->GetKey(SDL_SCANCODE_G) == KEY_UP)
	//{
	//	gravityScale = -gravityScale;
	//	pBody->body->SetGravityScale(gravityScale);
	//	pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, (float)gravityScale/2});
	//}

	pBody->body->SetLinearVelocity({ relativeVelocity_X , pBody->body->GetLinearVelocity().y });

	// God Mode
	if (_app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godMod = !godMod;
	}

	if (godMod && _app->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
	{
		pBody->SetSensor(true);
	}
	if (godMod && _app->input->GetKey(SDL_SCANCODE_B) == KEY_UP)
	{
		pBody->SetSensor(false);
	}

	// Fall faster
	if ((_app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || _app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
		&& skillCoolDown <= 0)
	{
		pBody->body->ApplyLinearImpulse({ 0, speed*2 }, { 0,0, }, true);
		usingSkill = true;
		skillCoolDown = totalSkillCoolDown;
	}

	// Move
	if (_app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || _app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		pBody->body->SetLinearVelocity({ relativeVelocity_X + speed,pBody->body->GetLinearVelocity().y });
	}
	else if(_app->input->GetKey(SDL_SCANCODE_D) == KEY_UP || _app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
	{
		pBody->body->SetLinearVelocity({ 0,pBody->body->GetLinearVelocity().y });
	}
	if (_app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || _app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		pBody->body->SetLinearVelocity({ relativeVelocity_X - speed,pBody->body->GetLinearVelocity().y });
	}
	else if (_app->input->GetKey(SDL_SCANCODE_A) == KEY_UP || _app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	{
		pBody->body->SetLinearVelocity({ 0,pBody->body->GetLinearVelocity().y  });
	}

	// Jump
	if (_app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN 
		|| _app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN
		|| _app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		Jump();
	}

	// Update animation state
	UpdatePlayerState();

	hitBoxSensor->Update();
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
	if (abs(pBody->body->GetLinearVelocity().x - relativeVelocity_X) > 0 || abs(pBody->body->GetLinearVelocity().x - relativeVelocity_X) < 0)
	{
		playerCurrentState = RUN;
		run.Update();
		return;
	}

	playerCurrentState = IDLE;
	idle.Update();
}

void Player::PostUpdate()
{
	if (!isAppear)
	{
		appearing.Update();

		renderObjects[5].section = appearing.GetCurrentFrame();
		renderObjects[5].destRect.x = GetDrawPosition().x + 5;
		renderObjects[5].destRect.y = GetDrawPosition().y + 6;

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
		renderObjects[i].destRect.x = GetDrawPosition().x + 5;
		renderObjects[i].destRect.y = GetDrawPosition().y + 6;

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
	if (col->gameObject->CompareTag("MobilePlatform"))
	{
		fallDetection = 10;

		if (col->gameObject->name == "specialPlatform")
		{
			jumpBlock = true;
		}
	}
	else if (col->gameObject->CompareTag("MobilePlatform_H"))
	{
		parent = col;
	}
	if (col->gameObject->CompareTag("WinTrigger"))
	{
		_app->scene->currentScene->isWin = true;
	}
}

void Player::OnCollisionExit(PhysBody* col)
{
	if (col->gameObject->CompareTag("MobilePlatform"))
	{
		fallDetection = 0.1f;

		if (col->gameObject->name == "specialPlatform")
		{
			jumpBlock = false;
		}
	}
	else if (col->gameObject->CompareTag("MobilePlatform_H"))
	{
		parent = nullptr;
		relativeVelocity_X = 0;
	}
}

void Player::OnTriggerEnter(PhysBody* trigger, PhysBody* col)
{
	if (col->gameObject->name == "spike" || col->gameObject->name == "saw")
	{
		Die();
	}

	if (col->gameObject->name == "bat" || col->gameObject->name == "chameleon")
	{
		if (!col->gameObject->isDie)
		{
			Die();
		}
	}
}

void Player::OnTriggerStay(PhysBody* trigger, PhysBody* col)
{
	if (col->gameObject->name == "chameleonAttack")
	{
		if (col->gameObject->enable && !isDead)
		{
			trigger->gameObject->OnCollisionExit(col);
			Die();
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

void Player::ResetJumpCount(int count)
{
	count = count > 2 ? 2 : count;
	jumpCount = count;
}

void Player::Jump()
{
	if (godMod)
	{
		pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, -jumpForce * 2 });
	}
	else if (jumpCount != 0 && !jumpBlock)
	{
		groundSensor->SetOffGround();

		pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, -jumpForce });

		jumpCount--;
	}
}

void Player::Die()
{
	if(!godMod)
	{
		//SDL_Delay(200);
		isDead = true;
	}
}
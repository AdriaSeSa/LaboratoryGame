#include "Player.h"

Player::Player(iPoint pos, std::string name, std::string tag, Application* app) : GameObject(name, tag, app)
{
	std::string texNames[5] = { "virtualGuyIdle","virtualGuyRun","virtualGuyJump","virtualGuyFall","virtualGuyDoubleJump" };

	for (int i = 0; i < 5; i++)
	{
		InitRenderObjectWithXml(texNames[i], i);
	}

	//Phys Body
	pBody = _app->physics->CreateRectangle(pos, 11, 16, this);

	// Puede ser que soluciona que el player pega en la pared
	//pBody = _app->physics->CreateCircle(pos.x, pos.y, 6, this);

	b2CircleShape circleShape;
	circleShape.m_radius = PIXELS_TO_METER(6);
	b2FixtureDef fixture1;
	fixture1.shape = &circleShape;
	fixture1.friction = 0;
	fixture1.density = 0;
	pBody->body->CreateFixture(&fixture1);

	/*b2PolygonShape box;
	box.SetAsBox(PIXELS_TO_METER(2) * 0.5f, PIXELS_TO_METER(2) * 0.5f);

	b2FixtureDef fixture2;
	fixture2.shape = &box;
	fixture2.density = 1.0f;

	pBody->body->CreateFixture(&fixture2);
	*/


	pBody->body->SetFixedRotation(true);

	pBody->body->SetBullet(true);

	pBody->body->GetFixtureList()->SetFriction(0.02f);

	pBody->body->SetGravityScale(2.0f);

	appliedFallForce = false;

	groundSensor = new GroundSensor(GetPosition() + groundSensorOffset, "PlayerGSensor", "GroundSensor", _app);

	hitBoxSensor = new HitboxSensor(GetPosition() + iPoint(-2,0),6, 8,this, "PlayerHitBox", "PlayerHitBox", _app);


	b2Filter physicFilter;
	physicFilter.groupIndex = -1;

	pBody->body->GetFixtureList()->SetFilterData(physicFilter);
	pBody->body->GetFixtureList()->GetNext()->SetFilterData(physicFilter);
	groundSensor->pBody->body->GetFixtureList()->SetFilterData(physicFilter);
	hitBoxSensor->pBody->body->GetFixtureList()->SetFilterData(physicFilter);

	hitBoxSensor->hits[0] = "saw";
	hitBoxSensor->hits[1] = "spike";

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

void Player::Update()
{	
	groundSensor->SetPosition(GetPosition() + groundSensorOffset);
	hitBoxSensor->SetPosition(GetPosition() + iPoint(1,0));

	isFalling = pBody->body->GetLinearVelocity().y > 0.1f;

	if (groundSensor->isOnGround)
	{
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

	if (_app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		pBody->body->SetLinearVelocity({ 0,speed });
	}

	if (_app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		if (jumpCount != 0)
		{
			groundSensor->SetOffGround();
			if (isFalling)
			{
				pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, -7.0f });

				//pBody->body->ApplyLinearImpulse({ 0,-1.7f }, { 0,0, }, true);
			}
			else 
			{
				pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, -7.0f });
				//pBody->body->ApplyLinearImpulse({ 0,-4.0f }, { 0,0, }, true);
			}
			
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
	if (pBody->body->GetLinearVelocity().y < 0)
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

void Player::OnCollisionEnter(PhysBody* col)
{

}
void Player::OnCollisionExit(PhysBody* col)
{
	//printf_s("PlayerColExit");
}

void Player::OnTriggerEnter(PhysBody* col)
{
	if (col->gameObject->name == "spike" || col->gameObject->name == "saw")
	{
		printf_s("HitBox!!!");
	}
}

void Player::PostUpdate()
{
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
}




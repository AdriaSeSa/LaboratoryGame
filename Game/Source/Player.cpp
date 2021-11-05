#include "Player.h"

Player::Player(iPoint pos, std::string name, std::string tag, Application* app) : GameObject(name, tag, app)
{
	//Phys Body
	pBody = _app->physics->CreateRectangle(pos, 12, 15, this);
	pBody->body->SetFixedRotation(true);

	pBody->body->SetBullet(true);

	pBody->body->GetFixtureList()->SetFriction(0.02f);

	pBody->body->SetGravityScale(2.0f);

	appliedFallForce = false;

	groundSensor = new GroundSensor(GetPosition() + groundSensorOffset, "PlayerGSensor", "GroundSensor", _app);

	//RenderObjects
	renderObjects[0].texture = _app->textures->Load("Assets/textures/Main Characters/Virtual Guy/Idle (32x32).png");
	renderObjects[1].texture = _app->textures->Load("Assets/textures/Main Characters/Virtual Guy/Run (32x32).png");
	renderObjects[2].texture = _app->textures->Load("Assets/textures/Main Characters/Virtual Guy/Jump (32x32).png");
	renderObjects[3].texture = _app->textures->Load("Assets/textures/Main Characters/Virtual Guy/Fall (32x32).png");
	renderObjects[4].texture = _app->textures->Load("Assets/textures/Main Characters/Virtual Guy/Double Jump (32x32).png");

	renderObjects[3].section = { 0,0,32,32 };
	renderObjects[4].section = { 0,0,32,32 };

	for (int i = 0; i < 5; i++)
	{
		renderObjects[i].scale = 0.5f;
	}

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

	isFalling = pBody->body->GetLinearVelocity().y > 0;

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

void Player::OnCollision(PhysBody* col)
{
	//printf_s("PlayerCol");
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
		int spriteOffsetX, spriteOffsetY;
		spriteOffsetX = 2;
		spriteOffsetY = 1;

		renderObjects[i].destRect.x = GetDrawPosition().x - spriteOffsetX;
		renderObjects[i].destRect.y = GetDrawPosition().y - spriteOffsetY;

		renderObjects[i].flip = pBody->body->GetLinearVelocity().x < 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

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
}




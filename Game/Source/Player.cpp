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
}

void Player::OnCollision(PhysBody* col)
{
	//printf_s("PlayerCol");
}

void Player::CleanUp()
{
	if (groundSensor != nullptr)
	{
		delete groundSensor;
		groundSensor = nullptr;
	}
}




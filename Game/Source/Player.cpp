#include "Player.h"

Player::Player(iPoint pos, std::string name, std::string tag, Application* app) : GameObject(name, tag, app)
{
	//Phys Body
	pBody = _app->physics->CreateRectangle(pos.x, pos.y, 12, 14);
	pBody->body->SetFixedRotation(true);
	pBody->body->SetBullet(true);

	pBody->body->GetFixtureList()->SetFriction(0);

	//pBody->body->SetGravityScale(4.0f);

	appliedFallForce = false;

}

void Player::Update()
{	
	if (pBody->body->GetLinearVelocity().y > 0 && !appliedFallForce)
	{
		//float fallVelocity = 4.0f;
		//pBody->body->SetLinearVelocity({ pBody->body->GetLinearVelocity().x, fallVelocity });

		pBody->body->ApplyLinearImpulse({ 0,1 }, { 0,0, }, true);
		appliedFallForce = true;
	}

	if (_app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		/*b2Vec2 pos = pBody->GetPosition();

		pos.x += 2;

		pos.x = PIXELS_TO_METER(pos.x);
		pos.y = PIXELS_TO_METER(pos.y);

		pBody->body->SetTransform(pos, 0);*/
		pBody->body->SetLinearVelocity({ 3,pBody->body->GetLinearVelocity().y });
	}
	else if(_app->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		pBody->body->SetLinearVelocity({ 0,pBody->body->GetLinearVelocity().y });
	}
	if (_app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		/*b2Vec2 pos = pBody->GetPosition();

		pos.x -= 2;

		pos.x = PIXELS_TO_METER(pos.x);
		pos.y = PIXELS_TO_METER(pos.y);

		pBody->body->SetTransform(pos, 0);*/
		pBody->body->SetLinearVelocity({ -3,pBody->body->GetLinearVelocity().y });
	}
	else if (_app->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		pBody->body->SetLinearVelocity({ 0,pBody->body->GetLinearVelocity().y  });
	}
	if (_app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		pBody->body->ApplyLinearImpulse({ 0,-1.5f }, {0,0,}, true);
		appliedFallForce = false;
	}

}

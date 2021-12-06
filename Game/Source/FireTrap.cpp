#include "FireTrap.h"
#include "Player.h"

FireTrap::FireTrap(iPoint position, std::string name, std::string tag, Application* app, bool isController)
	:GameObject(name, tag, app)
{
	this->isController = isController;

	// Init renderObject
	std::string texNames[2] = { "Flash","On" };

	for (int i = 0; i < 2; i++)
	{
		texNames[i] = name + texNames[i];
		InitRenderObjectWithXml(texNames[i], i);
	}

	// Init Anim
	for (int i = 0; i < 2; i++)
	{
		fireFlash.PushBack({ i * renderObjects[0].destRect.w,0,renderObjects[0].destRect.w,renderObjects[0].destRect.h });
	}
	fireFlash.hasIdle = false;
	fireFlash.loop = true;
	fireFlash.speed = 0.2f;

	for (int i = 0; i < 3; i++)
	{
		fireOn.PushBack({ i * renderObjects[1].destRect.w,0,renderObjects[1].destRect.w,renderObjects[1].destRect.h });
	}
	fireOn.hasIdle = false;
	fireOn.loop = true;
	fireOn.speed = 0.3f;
	renderObjects[1].draw = false;

	// Init pBody
	pBody = _app->physics->CreateRectangle({ position.x, position.y + 8 }, 16, 16, this);
	pBody->body->SetType(b2BodyType::b2_kinematicBody);

	hitBox = new HitboxSensor({ position.x, position.y }, 10, renderObjects[0].destRect.h, this, "fireTrapSensor", "FireTrapSensor", app);
	
	hitBox->hits[0] = "PlayerGSensor";

	startPos = GetPosition();
}

FireTrap::~FireTrap()
{
	if (hitBox != nullptr)
	{
		delete hitBox;
		hitBox = nullptr;
	}
}

void FireTrap::Reset()
{
	if(!isController)
	{
		flashDuration = 0;
		fireDuration = 0;
		SetPosition(startPos);
		hitBox->SetPosition({ startPos.x,startPos.y - 8 });
		SetLinearVelocity({ 0,0 });
	}
}

void FireTrap::Update()
{
	if(activeteFire)
	{
		if (flashDuration > 0)
		{
			flashDuration--;
		}
		else if (fireDuration > 0)
		{
			fireDuration--;
		}
		else
		{
			activeteFire = false;
			//pBody->SetSensor(true);
		}
	}

	if (isController && HasPlayerIn())
	{
		FireOn(100, 50);
	}
	else if (hitBox != nullptr)
	{
		b2Vec2 vel = GetLinearVelocity();
		hitBox->SetLinearVelocity(vel);
	}
}

void FireTrap::PostUpdate()
{
	if (activeteFire)
	{
		if (flashDuration > 0)
		{
			// Flashing
			fireFlash.Update();
			renderObjects[1].draw = false;
			renderObjects[0].draw = true;
			renderObjects[0].section = fireFlash.GetCurrentFrame();
		}
		else
		{
			// Fire
			fireOn.Update();
			renderObjects[0].draw = false;
			renderObjects[1].draw = true;
			renderObjects[1].section = fireOn.GetCurrentFrame();
			if (player != nullptr)
			{
				player->Die();
				player = nullptr;
			}
		}
	}
	else
	{
		renderObjects[1].draw = false;
		renderObjects[0].draw = true;
		renderObjects[0].section = { 0,0, renderObjects[0].destRect.w, renderObjects[0].destRect.h };
	}

	for (int i = 0; i < MAX_GAMEOBJECT_TEXTURES; i++)
	{
		if (renderObjects[i].texture != nullptr && renderObjects[i].draw)
		{
			renderObjects[i].destRect.x = GetDrawPosition().x;
			renderObjects[i].destRect.y = GetDrawPosition().y - 8;
			renderObjects[i].rotation = GetDegreeAngle();

			_app->renderer->AddTextureRenderQueue(renderObjects[i].texture, { renderObjects[i].destRect.x,renderObjects[i].destRect.y },
				renderObjects[i].section, renderObjects[i].scale, renderObjects[i].layer, renderObjects[i].orderInLayer,
				renderObjects[i].rotation, renderObjects[i].flip, renderObjects[i].speedRegardCamera);
		}
	}
}

void FireTrap::OnTriggerEnter(PhysBody* col)
{
	if (col->gameObject->CompareTag("GroundSensor"))
	{
		if (player == nullptr)
		{
			// NO DETECTA COLISION  DE PLAYER !!!
			GroundSensor* g = (GroundSensor*)col->gameObject;

			player = (Player*)g->father;
		}
	}
}

void FireTrap::OnTriggerExit(PhysBody* col)
{
	if (col->gameObject->CompareTag("GroundSensor"))
	{
		player = nullptr;
	}
}

void FireTrap::CleanUp()
{
	if (hitBox != nullptr)
	{
		delete hitBox;
		hitBox = nullptr;
	}
}

bool FireTrap::FireOn(int flashDuration,int fireDuration)
{
	if(!activeteFire)
	{
		activeteFire = true;
		this->flashDuration = flashDuration;
		this->fireDuration = fireDuration;
		return true;
	}
	return false;
}

bool FireTrap::HasPlayerIn()
{
	return player == nullptr ? false : true;
}

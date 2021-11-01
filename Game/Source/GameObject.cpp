#include "GameObject.h"
#include "ModulePhysics.h"

GameObject::GameObject()
{
}

//TODO: Poner Applicationi* en constructor
GameObject::GameObject(std::string name, std::string tag , Application* _app)
{
	this->name = name;
	this->_app = _app;
	this->tag = tag;
}

GameObject::GameObject(GameObject& obj)
{
	this->name = obj.name;
	this->tag = obj.tag;
	this->pBody = obj.pBody;
}

GameObject::~GameObject()
{
	if(pBody!=nullptr)
	{
		delete pBody;
		pBody = nullptr;
	}
}

void GameObject::OnCollision(PhysBody* col)
{
}

void GameObject::Start()
{
	//Cargar texturas
}

void GameObject::PreUpdate()
{
}

void GameObject::Update()
{
}

void GameObject::PostUpdate()
{
	// Dibujar texturas
	for (int i = 0; i < MAX_GAMEOBJECT_TEXTURES; i++)
	{
		if (renderObjects[i].texture != nullptr)
		{
			renderObjects[i].renderRect.x = GetPosition().x;
			renderObjects[i].renderRect.y = GetPosition().y;
			renderObjects[i].rotation = GetDegreeAngle();
			
			_app->renderer->AddTextureRenderQueue(renderObjects[i]);
		}
	}
}

void GameObject::CleanUp()
{
}

bool GameObject::CompareTag(std::string tag)
{
	if(this->tag == tag)
	{
		return true;
	}

	return false;
}

float GameObject::GetDegreeAngle()
{
	if (pBody != nullptr)
	{
		float agle = 0;

		agle = (pBody->body->GetAngle() * 180) / b2_pi;

		return agle;
	}
	
	return this->rotation;
}

iPoint GameObject::GetPosition()
{
	if (pBody != nullptr)
	{
		b2Vec2 position;

		position = pBody->body->GetPosition();
		position.x = METERS_TO_PIXELS(position.x);
		position.y = METERS_TO_PIXELS(position.y);

		return { (int)position.x, (int)position.y };
	}

	return this->position;
}

void GameObject::SetPosition(iPoint pos)
{
	if (pBody != nullptr)
	{
		pBody->body->SetTransform(b2Vec2((PIXELS_TO_METER(pos.x)), (PIXELS_TO_METER(pos.y))), pBody->body->GetAngle());
	}
	else
	{
		this->position = pos;
	}
}

void GameObject::SetRotation(float angle)
{
	if (pBody != nullptr)
	{
		pBody->body->SetTransform(pBody->body->GetPosition(), DEGTORAD * angle);
	}
	else
	{
		this->rotation = angle;
	}
}

void GameObject::InitRenderObjectWithXml()
{
	renderObjects[0].texture = _app->textures->Load(name, true);
	renderObjects[0].renderRect.w = _app->textures->config.child(name.c_str()).attribute("width").as_int();
	renderObjects[0].renderRect.h = _app->textures->config.child(name.c_str()).attribute("height").as_int();
}

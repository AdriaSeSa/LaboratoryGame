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
	for (int i = 0; i < MAX_GAMEOBJECT_TEXTURES; i++)
	{
		if (renderObjects[i].section != nullptr)
		{
			delete renderObjects[i].section;
			renderObjects[i].section = nullptr;
		}
	}

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
	// DIbujar texturas
	for (int i = 0; i < MAX_GAMEOBJECT_TEXTURES; i++)
	{
		if (renderObjects[i].texture != nullptr)
		{
			if (pBody != nullptr && renderObjects[i].followPhysBody)
			{
				renderObjects[i].renderRect.x = GetDrawPos().x;
				renderObjects[i].renderRect.y = GetDrawPos().y;

				if (renderObjects[i].rotationEnabled)
				{
					renderObjects[i].rotation = GetDegreeAngle();
				}
			}

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

iPoint GameObject::GetDrawPos()
{
	if (this->pBody == nullptr)
	{
		LOG("GameObject has no PhysBody!"); 
		return iPoint(0, 0);
	}

	b2Vec2 pos;
	pos = pBody->body->GetPosition();

	pos.x = METERS_TO_PIXELS(pos.x - pBody->width);
	pos.y = METERS_TO_PIXELS(pos.y - pBody->height);

	return iPoint(pos.x,pos.y);
}

float GameObject::GetDegreeAngle()
{
	float agle = 0;

	agle = (pBody->body->GetAngle() * 180) / b2_pi;

	return agle;
}

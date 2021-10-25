#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include "Point.h"
#include "Application.h"
struct SDL_Texture;
class PhysBody;

#define MAX_GAMEOBJECT_TEXTURES 5

class GameObject 
{
public:

	GameObject();

	GameObject(std::string name = "Default", std::string tag = "None", Application* _app = nullptr);

	GameObject(GameObject& obj);

	~GameObject();

	virtual void OnCollision(PhysBody* col);

	virtual void Start();

	virtual void PreUpdate();

	virtual void Update();

	virtual void PostUpdate();

	virtual void CleanUp();

	bool CompareTag(std::string tag);

	iPoint GetDrawPos();

	float GetDegreeAngle();

public:

	std::string name;

	std::string tag;

	RenderObject renderObjects[MAX_GAMEOBJECT_TEXTURES];

	PhysBody* pBody = nullptr;

	PhysBody* pBody2 = nullptr;

	Application* _app = nullptr;

	bool pendingToDelete = false;
};

#endif // !GAMEOBJECT_H
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

	iPoint GetDrawPosition();

	/// <summary>
	/// Si exixte pBody devuelve angluo de pBody, si no el de GameObject 
	/// </summary>
	/// <returns>angulo en degradado</returns>
	float GetDegreeAngle();
	
	/// <summary>
	/// Si exixte pBody devuelve position de pBody, si no el de GameObject (px)
	/// </summary>
	/// <returns>position en pixel</returns>
	iPoint GetPosition();

	/// <summary>
	/// Si exixte pBody, cambia la position de pBody, si no la de GameObject
	/// </summary>
	/// <param name="pos">= position in pixel</param>
	void SetPosition(iPoint pos);

	/// <summary>
	/// Si exixte pBody, cambia el amgulo de pBody, si no el de GameObject
	/// </summary>
	/// <param name="angle">= angle in deg</param>
	void SetRotation(float angle);

protected:

	void InitRenderObjectWithXml();

private :
	iPoint position = { 0,0 };

	float rotation = 0;

protected:
	Application* _app = nullptr;

public:

	std::string name;

	std::string tag;

	PhysBody* pBody = nullptr;

	RenderObject renderObjects[MAX_GAMEOBJECT_TEXTURES];

	bool pendingToDelete = false;

	bool adjustToGrid = false;
};

#endif // !GAMEOBJECT_H
#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"
#include <math.h>

class GameObject;

#define GRAVITY_X 0.0f
#define GRAVITY_Y -10.0f

#define PIXELS_PER_METER 50.0f
#define METERS_PER_PIXELS 0.02f

#define METERS_TO_PIXELS(m) ((int)floor(PIXELS_PER_METER * m) )
#define PIXELS_TO_METER(p) ((float)METERS_PER_PIXELS * p)


class PhysBody
{
public:

	PhysBody() : body(NULL)
	{}

	PhysBody(GameObject* g) : body(NULL)
	{
		gameObject = g;
	}

	~PhysBody()
	{
		if (body != nullptr)
		{
			body->GetWorld()->DestroyBody(body);
		}
	}

	void GetPosition(int& x, int& y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;
	void GetCenterPosition(int& x, int& y) const;
public:
	int width, height;
	bool chainLoop = false;
	b2Body* body = nullptr;
	GameObject* gameObject = nullptr;

	//Module* listenerModule = nullptr;
};

class ModulePhysics : public Module, public b2ContactListener
{
public:

	b2World* world = nullptr;
	

	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	UpdateStatus PreUpdate();
	UpdateStatus Update();
	UpdateStatus PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, GameObject* gameObject = nullptr, bool isSensor = false);
	PhysBody* CreateRectangle(int x, int y, int width, int height);
	PhysBody* CreateRectangleSensor(iPoint pos, int width, int height);
	PhysBody* CreateChainObj(int x, int y, int* points, int size, bool loop);
	b2Vec2 Perp(b2Vec2 vec1);
	/*b2Vec2 Normalise(b2Vec2 vecToNormalise);*/
	void DotProductAngle(b2Vec2 v1,b2Vec2 v2, float& angle);
	
	void Pause();

	void BeginContact(b2Contact* contact) override;

	void ShapesRender();

private:
	b2Body* mouseBody = nullptr;
	b2MouseJoint* mouseJoint = nullptr;

	bool debug;

	bool pause = false;
};

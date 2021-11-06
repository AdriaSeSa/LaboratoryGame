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

#define METERS_TO_PIXELS(m) ((int)(PIXELS_PER_METER * m) / 2)
#define PIXELS_TO_METER(p) ((float)METERS_PER_PIXELS * p * 2)


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
			body = nullptr;
		}
	}

	void SetSensor(bool isSensor)
	{
		body->GetFixtureList()->SetSensor(isSensor);

		this->isSensor = isSensor;
	}

	bool GetSenser() const { return isSensor; }

	void GetPosition(int& x, int& y) const;
	b2Vec2 GetPosition();
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;
	void GetCenterPosition(int& x, int& y) const;
public:
	int width, height;
	bool chainLoop = false;
	bool isSensor = false;
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

	bool Init(pugi::xml_node&);
	bool Start();
	UpdateStatus PreUpdate() override;
	UpdateStatus Update();
	UpdateStatus PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, GameObject* gameObject = nullptr, bool isSensor = false);
	PhysBody* CreateRectangle(iPoint pos, int width, int height, GameObject* gameObject = nullptr);
	PhysBody* CreateRectangleSensor(iPoint pos, int width, int height, GameObject* g = nullptr);
	PhysBody* CreateLine(b2Vec2 startPos, b2Vec2 dir, GameObject* gameObject);
	PhysBody* CreateChainObj(int x, int y, int* points, int size, bool loop);
	PhysBody* CreateEdgeObj(int x, int y, int* points, int size, bool loop);
	b2Vec2 Perp(b2Vec2 vec1);
	/*b2Vec2 Normalise(b2Vec2 vecToNormalise);*/
	void DotProductAngle(b2Vec2 v1,b2Vec2 v2, float& angle);
	
	void Pause();

	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;

	void ShapesRender();

private:
	b2Body* mouseBody = nullptr;
	b2MouseJoint* mouseJoint = nullptr;

	bool debug;

	bool pause = false;
};

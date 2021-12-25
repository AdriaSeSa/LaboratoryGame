#pragma once
class Application;
class GameObject;
#include "Point.h"

class Camera
{
public:
	Camera(Application* app, GameObject* target = nullptr);

	void Init(GameObject* target, int width, int height);

	void Start();

	void Update();

	void UpdatePosition();

	void SetTarget(GameObject * target);

	void ReleaseTarget();

private:

	iPoint GetCenter();

	Application* App = nullptr;

	GameObject* targetv;

	// use for offset with target
	int pivotX = 0, pivotY = 0;

	int distanceFromTarget = 0;

public:
	int x = 0;
	int y = 0;

	int cameraSpeed = 1;

	// mas grande, mas lento se mueve la camera
	int cameraDelay = 20;

	// decidir si se mueve o no
	int moveX = 0, moveY = 1;

	int mapHeight = 0, mapWidth = 0;
};
#pragma once

#include "Point.h"

class Application;

enum class UIShape
{
	CIRCLE,
	BOX
};

struct RectangleShape
{
	int w = 0;
	int h = 0;
};

struct CircleShape
{
	int radius = 0;
};

class GUI
{
public:
	GUI(Application* app);

	void InitAsBox(int x, int y, int w, int h);

	void InitAsCircle(int x, int y, int radius);

public:
	bool CheckOnMouse();

public:
	Application* _app;

	iPoint position = { 0,0 };

	// Box Shape
	RectangleShape boxShape;

	// Circle Shape
	CircleShape circleShape;

	UIShape currentShape = UIShape::CIRCLE;
};


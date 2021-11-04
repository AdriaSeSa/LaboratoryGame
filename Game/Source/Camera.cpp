#include "Camera.h"
#include "Application.h"
#include "GameObject.h"

Camera::Camera(Application* app, GameObject* target)
{
	App = app;
	this->target = target;
}

void Camera::Start()
{
	pivotX = App->window->width / 2;
	pivotY = App->window->height / 2;
}

void Camera::Update()
{
	UpdatePosition();

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
	{
		y -= cameraSpeed;
		printf_s("Camera_X: %d, Camera_Y: %d\n", x, y);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		y += cameraSpeed;
		printf_s("Camera_X: %d, Camera_Y: %d\n", x, y);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		x -= cameraSpeed;
		printf_s("Camera_X: %d, Camera_Y: %d\n",x, y);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		x += cameraSpeed;
		printf_s("Camera_X: %d, Camera_Y: %d\n", x, y);
	}
}

void Camera::UpdatePosition()
{
	if (target != nullptr)
	{
		// Update Y
		int targetPosY = target->GetPosition().y * App->window->scale;
		targetPosY = targetPosY - pivotY;


		if (targetPosY > y )
		{
			int distance = abs(targetPosY - y);
			y+=distance /20;
		}
		else if (targetPosY < y)
		{
			int distance = abs(targetPosY - y);
			y-= distance / 20;
		}

		if (y < 0) y = 0;
		else if (y > mapHeight) y = mapHeight;

		// Update X

		int targetPosX = target->GetPosition().x * App->window->scale;
		targetPosX = targetPosX - pivotX;
	

		if (targetPosX > x)
		{
			int distance = abs(targetPosX - x);
			x += distance / 20;
		}
		else if (targetPosX < x)
		{
			int distance = abs(targetPosX - x);
			x -= distance / 20;
		}

		if (x < 0) x = 0;
		else if (x > mapWidth) x = mapWidth;
	}
	
}

void Camera::SetTarget(GameObject* target)
{
	this->target = target;
}

iPoint Camera::CalculateDistanceToTarget()
{
	distanceFromTarget = target->GetPosition().DistanceTo({ x + pivotX, y + pivotY });

	iPoint vec = iPoint(target->GetPosition().x, target->GetPosition().y) 
		- iPoint(GetCenter().x * App->window->scale, GetCenter().y * App->window->scale);

	return vec;
}

iPoint Camera::GetCenter()
{
	return {x + (int)App->window->width/2, y + 160 };
}



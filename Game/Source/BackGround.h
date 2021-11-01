#pragma once
#include "GameObject.h"
class BackGround :  public GameObject
{
public:
	BackGround(std::string name, std::string tag, Application* app);

	void PostUpdate() override;

private:
	int WindowHeight = 640;
	int WindowWidth = 320;
};


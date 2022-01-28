#pragma once

#include "List.h"

class GUI;
class Application;

class GUIPanel
{
protected:
	Application* _app = nullptr;

	List<GUI*> guiList;

public:

	GUIPanel(Application* app);

	virtual void CheckInteractions();

	virtual void Update();

	virtual void PostUpdate();

	virtual void CleanUp();

	bool changeScene = false;
};


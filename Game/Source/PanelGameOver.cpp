#include "PanelGameOver.h"
#include "GUIButton.h"
#include "Application.h"

PanelGameOver::PanelGameOver(Application* app) : GUIPanel(app)
{
	retryButton = new GUIButton(_app, { 105, 180 }, 96, 32, "Assets/textures/UI/Retry.png");
	guiList.add(retryButton);

	mainMenuButton = new GUIButton(_app, { 105, 220 }, 96, 32, "Assets/textures/UI/Menu.png");
	guiList.add(mainMenuButton);

	quitButton = new GUIButton(_app, { 120, 260 }, 64, 32, "Assets/textures/UI/Exit.png");
	guiList.add(quitButton);
}

void PanelGameOver::CheckInteractions()
{
	if (retryButton->doAction)
	{
		retryButton->doAction = false;
		_app->scene->ChangeCurrentSceneRequest(_app->scene->lastLevel);
	}
	if (mainMenuButton->doAction)
	{
		mainMenuButton->doAction = false;
		_app->scene->ChangeCurrentSceneRequest(0);
	}
	if (quitButton->doAction)
	{
		_app->ExitGame();
	}
}

void PanelGameOver::Update()
{
	for (int i = 0; i < guiList.count(); i++)
	{
		if (guiList[i] != nullptr)
		{
			guiList[i]->Update();
		}
	}
	CheckInteractions();
}

void PanelGameOver::PostUpdate()
{
	for (int i = 0; i < guiList.count(); i++)
	{
		if (guiList[i] != nullptr)
		{
			guiList[i]->PostUpdate();
		}
	}
}

void PanelGameOver::CleanUp()
{
	guiList.clearPtr();
}

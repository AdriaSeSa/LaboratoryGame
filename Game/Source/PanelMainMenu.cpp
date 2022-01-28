#include "PanelMainMenu.h"
#include "GUIButton.h"
#include "GUICheckbox.h"
#include "GUISlider.h"
#include "Application.h"
#include "SceneMainMenu.h"

PanelMainMenu::PanelMainMenu(Application* app, SceneMainMenu* scene) : GUIPanel(app)
{
	this->scene = scene;
	currentScreen = 0;

	// Main menu
	startButton = new GUIButton(_app, { 95, 150 }, 96, 32, "Assets/textures/UI/StartButton4.png");
	guiList.add(startButton);

	quitButton = new GUIButton(_app, { 95, 250 }, 68, 32, "Assets/textures/UI/Exit.png");
	guiList.add(quitButton);

	settingsButton = new GUIButton(_app, { 95, 200 }, 128, 32, "Assets/textures/UI/Settings.png");
	guiList.add(settingsButton);

	// Select Level
	level1Button = new GUIButton(_app, { 55, 100 }, 128, 64, "Assets/textures/UI/StartButton.png");
	selectLevelGUI.add(level1Button);

	level2Button = new GUIButton(_app, { 200, 100 }, 128, 64, "Assets/textures/UI/StartButton.png");
	selectLevelGUI.add(level2Button);

	backToMainMenu1 = new GUIButton(_app, { 0, 0 }, 128, 64, "Assets/textures/UI/StartButton.png");
	selectLevelGUI.add(backToMainMenu1);

	// Settings
	fullScreenCheck = new GUICheckbox(_app, { 50,50 }, 48, 48, "Assets/textures/UI/CheckboxTest.png");
	settingsGUI.add(fullScreenCheck);

	vSyncCheck = new GUICheckbox(_app, { 50,120 }, 48, 48, "Assets/textures/UI/CheckboxTest.png");
	settingsGUI.add(vSyncCheck);

	musicSlider = new GUISlider(_app, { 50, 180 }, 128, 16, "Assets/textures/UI/SliderTest.png");
	musicSlider->CreateDefaultBtn();
	settingsGUI.add(musicSlider);

	sfxSlider = new GUISlider(_app, { 50, 230 }, 128, 16, "Assets/textures/UI/SliderTest.png");
	sfxSlider->CreateDefaultBtn();
	settingsGUI.add(sfxSlider);

	backToMainMenu2 = new GUIButton(_app, { 0, 0 }, 128, 64, "Assets/textures/UI/StartButton.png");
	settingsGUI.add(backToMainMenu2);

	InitializeSettings();
}

void PanelMainMenu::CheckInteractions()
{
	switch (currentScreen)
	{
	case 0:
		if (startButton->doAction)
		{
			scene->ChangeScreen(1);
			currentScreen = 1;
			startButton->doAction = false;
			break;
		}
		else if (settingsButton->doAction)
		{
			scene->ChangeScreen(2);
			currentScreen = 2;
			settingsButton->doAction = false;
			break;
		}
		else if (quitButton->doAction)
		{
			scene->exit = true;
			break;
		}
	case 1:
		if (level1Button->doAction)
		{
			_app->scene->ChangeCurrentSceneRequest(2);
			break;
		}
		else if (level2Button->doAction)
		{
			_app->scene->ChangeCurrentSceneRequest(3);
			break;
		}
		else if (backToMainMenu1->doAction)
		{
			scene->ChangeScreen(0);
			currentScreen = 0;
			backToMainMenu1->doAction = false;
			break;
		}
		break;
	case 2:
		// fullscreen = fullScreenCheck.isActive
		// vsync = vSyncCheck.isActive
		if (backToMainMenu2->doAction)
		{
			scene->ChangeScreen(0);
			currentScreen = 0;
			backToMainMenu2->doAction = false;
		}

		Mix_VolumeMusic(60 * musicSlider->GetValue());
		Mix_Volume(-1, 60 * sfxSlider->GetValue());

		if (vSyncCheck->isActive && !_app->vsync)
		{
			_app->renderer->ToggleVsync(true);
		}
		else if (!vSyncCheck->isActive && _app->vsync)
		{
			_app->renderer->ToggleVsync(false);
		}

		if (fullScreenCheck->isActive && !_app->FullScreenDesktop)
		{
			_app->window->ToggleFullScreen(true);
		}
		else if (!fullScreenCheck->isActive && _app->FullScreenDesktop)
		{
			_app->window->ToggleFullScreen(false);
		}
		break;
	}
	
}

void PanelMainMenu::Update()
{
	switch (currentScreen)
	{
	case 0:
		for (int i = 0; i < guiList.count(); i++)
		{
			if (guiList[i] != nullptr)
			{
				guiList[i]->Update();
			}
		}
		break;
	case 1:
		for (int i = 0; i < selectLevelGUI.count(); i++)
		{
			if (selectLevelGUI[i] != nullptr)
			{
				selectLevelGUI[i]->Update();
			}
		}
		break;
	case 2:
		for (int i = 0; i < settingsGUI.count(); i++)
		{
			if (settingsGUI[i] != nullptr)
			{
				settingsGUI[i]->Update();
			}
		}
		break;
	}
	CheckInteractions();
}

void PanelMainMenu::PostUpdate()
{
	switch (currentScreen)
	{
	case 0:
		for (int i = 0; i < guiList.count(); i++)
		{
			if (guiList[i] != nullptr)
			{
				guiList[i]->PostUpdate();
			}
		}
		break;
	case 1:
		for (int i = 0; i < selectLevelGUI.count(); i++)
		{
			if (selectLevelGUI[i] != nullptr)
			{
				selectLevelGUI[i]->PostUpdate();
			}
		}
		break;
	case 2:
		for (int i = 0; i < settingsGUI.count(); i++)
		{
			if (settingsGUI[i] != nullptr)
			{
				settingsGUI[i]->PostUpdate();
			}
		}
		break;
	}
	
}

void PanelMainMenu::CleanUp()
{
	SaveSettings();
	guiList.clearPtr();
	selectLevelGUI.clearPtr();
	settingsGUI.clearPtr();
}

void PanelMainMenu::InitializeSettings()
{
	pugi::xml_node n = _app->saveF.child("game_state").child("settings");

	fullScreenCheck->ChangeState(n.attribute("fullScreen").as_bool(false));
	vSyncCheck->ChangeState(n.attribute("vSync").as_bool(true));
	musicSlider->SetValue(n.attribute("music").as_float(0.5));
	sfxSlider->SetValue(n.attribute("sfx").as_float(0.5));
}

void PanelMainMenu::SaveSettings()
{
	pugi::xml_node n = _app->saveF.child("game_state").child("settings");

	n.attribute("fullScreen") = fullScreenCheck->isActive;
	n.attribute("vSync") = vSyncCheck->isActive;
	n.attribute("music") = musicSlider->GetValue();
	n.attribute("sfx") = sfxSlider->GetValue();

	_app->saveF.save_file(SAVE_STATE_FILENAME);

}

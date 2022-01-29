#include "PanelMainMenu.h"
#include "GUIButton.h"
#include "GUICheckbox.h"
#include "GUISlider.h"
#include "Application.h"
#include "SceneMainMenu.h"

PanelMainMenu::PanelMainMenu(Application* app, SceneMainMenu* scene) : GUIPanel(app)
{
	this->scene = scene;
	currentScreen = MenuScreens::MAIN_MENU;

	// Main menu
	startButton = new GUIButton(_app, { 110, 140 }, 96, 32, "Assets/textures/UI/StartButton4.png");
	guiList.add(startButton);

	if (CheckForSavedFile())
	{
		continueButton = new GUIButton(_app, { 95, 190 }, 128, 32, "Assets/textures/UI/Continue.png");
		guiList.add(continueButton);
	}
	else 
	{
		// Boton gris
	}

	quitButton = new GUIButton(_app, { 240, 280 }, 68, 32, "Assets/textures/UI/Exit.png");
	guiList.add(quitButton);

	settingsButton = new GUIButton(_app, { 95, 240 }, 128, 32, "Assets/textures/UI/Settings.png");
	guiList.add(settingsButton);

	creditsButton = new GUIButton(_app, { 10, 280 }, 96, 32, "Assets/textures/UI/Credit.png");
	guiList.add(creditsButton);

	// Credits
	backToMainMenu3 = new GUIButton(_app, { 0, 0 }, 32, 32, "Assets/textures/UI/ReturnBtn.png");
	creditsGUI.add(backToMainMenu3);

	// Select Level
	level1Button = new GUIButton(_app, { 60, 80 }, 48, 48, "Assets/textures/Menu/Levels1Btn.png");
	selectLevelGUI.add(level1Button);

	level2Button = new GUIButton(_app, { 200, 80 }, 48, 48, "Assets/textures/Menu/Levels2Btn.png");
	selectLevelGUI.add(level2Button);

	levelSecretButton = new GUIButton(_app, { 130, 140 }, 48, 48, "Assets/textures/Menu/LevelsLockedBtn.png");
	selectLevelGUI.add(levelSecretButton);

	backToMainMenu1 = new GUIButton(_app, { 0, 0 }, 32, 32, "Assets/textures/UI/ReturnBtn.png");
	selectLevelGUI.add(backToMainMenu1);

	character1Check = new GUICheckbox(_app, { 60, 250 }, 32, 32, "Assets/textures/UI/character1.png");
	selectLevelGUI.add(character1Check);
	character1Check->ChangeState(true);

	character2Check = new GUICheckbox(_app, { 120, 250 }, 32, 32, "Assets/textures/UI/character2.png");
	selectLevelGUI.add(character2Check);

	// Settings
	fullScreenCheck = new GUICheckbox(_app, { 50,220 }, 32, 32, "Assets/textures/UI/Checkbox.png");
	settingsGUI.add(fullScreenCheck);

	vSyncCheck = new GUICheckbox(_app, { 50,264 }, 32, 32, "Assets/textures/UI/Checkbox.png");
	settingsGUI.add(vSyncCheck);

	musicSlider = new GUISlider(_app, { 55, 110 }, 128, 16, "Assets/textures/UI/Slider.png");
	musicSlider->CreateGUIBtn(new GUIButton(_app, { 55, 110 }, 10, 16, "Assets/textures/UI/Slider_Btn.png"));
	settingsGUI.add(musicSlider);

	sfxSlider = new GUISlider(_app, { 55, 170 }, 128, 16, "Assets/textures/UI/Slider.png");
	sfxSlider->CreateGUIBtn(new GUIButton(_app, { 55, 170 }, 10, 16, "Assets/textures/UI/Slider_Btn.png"));
	settingsGUI.add(sfxSlider);

	backToMainMenu2 = new GUIButton(_app, { 0, 0 }, 32, 32, "Assets/textures/UI/ReturnBtn.png");
	settingsGUI.add(backToMainMenu2);

	InitializeSettings();

	settingsBackgroundTexture = _app->textures->Load("Assets/textures/UI/Panel_setting.png");
	creditsBackgroundTexture = _app->textures->Load("Assets/textures/UI/Panel_Credit.png");
}

void PanelMainMenu::CheckInteractions()
{
	switch (currentScreen)
	{
	case MenuScreens::MAIN_MENU:
		if (startButton->doAction)
		{
			scene->ChangeScreen(1);
			currentScreen = (MenuScreens)1;
			startButton->doAction = false;
			break;
		}
		else if (continueButton != nullptr && continueButton->doAction)
		{
			continueButton->doAction = false;

			pugi::xml_node node = _app->saveF.child("game_state").child("debug").child("lastState");
			_app->scene->ChangeCurrentSceneRequest(node.attribute("scene").as_int());

			break;
		}
		else if (settingsButton->doAction)
		{
			scene->ChangeScreen(2);
			currentScreen = (MenuScreens)2;
			settingsButton->doAction = false;
			break;
		}
		else if (creditsButton->doAction)
		{
			scene->ChangeScreen(3);
			currentScreen = (MenuScreens)3;
			creditsButton->doAction = false;
		}
		else if (quitButton->doAction)
		{
			scene->exit = true;
			break;
		}
	case MenuScreens::SELECTLEVEL:
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
			currentScreen = (MenuScreens)0;
			backToMainMenu1->doAction = false;
			break;
		}

		if (character1Check->isActive && _app->scene->characterIndex != 0)
		{
			_app->scene->characterIndex = 0;
			character2Check->ChangeState(false);
		}
		else if (character2Check->isActive && _app->scene->characterIndex != 1)
		{
			_app->scene->characterIndex = 1;
			character1Check->ChangeState(false);
		}

		break;
	case MenuScreens::SETTINGS:
		if (backToMainMenu2->doAction)
		{
			scene->ChangeScreen(0);
			currentScreen = (MenuScreens)0;
			backToMainMenu2->doAction = false;
		}

		Mix_VolumeMusic(60 * musicSlider->GetValue());
		Mix_Volume(-1, 200 * sfxSlider->GetValue());

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
	case MenuScreens::CREDIT:
		if (backToMainMenu3->doAction)
		{
			scene->ChangeScreen(0);
			currentScreen = (MenuScreens)0;
			backToMainMenu3->doAction = false;
		}
		break;
	}
}

void PanelMainMenu::Update()
{
	switch (currentScreen)
	{
	case MenuScreens::MAIN_MENU:
		for (int i = 0; i < guiList.count(); i++)
		{
			if (guiList[i] != nullptr)
			{
				guiList[i]->Update();
			}
		}
		break;
	case MenuScreens::SELECTLEVEL:
		for (int i = 0; i < selectLevelGUI.count(); i++)
		{
			if (selectLevelGUI[i] != nullptr)
			{
				selectLevelGUI[i]->Update();
			}
		}
		break;
	case MenuScreens::SETTINGS:
		for (int i = 0; i < settingsGUI.count(); i++)
		{
			if (settingsGUI[i] != nullptr)
			{
				settingsGUI[i]->Update();
			}
		}
		break;
	case MenuScreens::CREDIT:
		for (int i = 0; i < creditsGUI.count(); i++)
		{
			if (creditsGUI[i] != nullptr)
			{
				creditsGUI[i]->Update();
			}
		}
		break;
	}
	CheckInteractions();
}

void PanelMainMenu::PostUpdate()
{
	if (currentScreen == MenuScreens::SETTINGS) _app->renderer->AddTextureRenderQueue(settingsBackgroundTexture, { 0,0 }, { 0,0,0,0 }, 1, 3, 5);
	if (currentScreen == MenuScreens::CREDIT) _app->renderer->AddTextureRenderQueue(creditsBackgroundTexture, { 0,0 }, { 0,0,0,0 }, 1, 3, 5);

	switch (currentScreen)
	{
	case MenuScreens::MAIN_MENU:
		for (int i = 0; i < guiList.count(); i++)
		{
			if (guiList[i] != nullptr)
			{
				guiList[i]->PostUpdate();
			}
		}
		break;
	case MenuScreens::SELECTLEVEL:
		for (int i = 0; i < selectLevelGUI.count(); i++)
		{
			if (selectLevelGUI[i] != nullptr)
			{
				selectLevelGUI[i]->PostUpdate();
			}
		}
		break;
	case MenuScreens::SETTINGS:
		for (int i = 0; i < settingsGUI.count(); i++)
		{
			if (settingsGUI[i] != nullptr)
			{
				settingsGUI[i]->PostUpdate();
			}
		}
		break;
	case MenuScreens::CREDIT:
		for (int i = 0; i < creditsGUI.count(); i++)
		{
			if (creditsGUI[i] != nullptr)
			{
				creditsGUI[i]->PostUpdate();
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
	creditsGUI.clearPtr();
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

bool PanelMainMenu::CheckForSavedFile()
{
	pugi::xml_node node = _app->saveF.child("game_state").child("debug").child("lastState");

	bool saved = node.attribute("isSaved").as_bool(false);

	return saved;
}

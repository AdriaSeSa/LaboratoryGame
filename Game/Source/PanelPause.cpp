#include "PanelPause.h"
#include "Application.h"
#include "GUIButton.h"
#include "GUICheckbox.h"
#include "GUISlider.h"

PanelPause::PanelPause(Application* app) : GUIPanel(app)
{
	currentScreen = 0;

	// Pause Screen
	resumeButton = new GUIButton(_app, { 95, 70 }, 128, 32, "Assets/textures/UI/Continue.png");
	guiList.add(resumeButton);

	settingsButton = new GUIButton(_app, { 95, 120 }, 128, 32, "Assets/textures/UI/Settings.png");
	guiList.add(settingsButton);

	backToTitleButton = new GUIButton(_app, { 110, 170 }, 96, 32, "Assets/textures/UI/Menu.png");
	guiList.add(backToTitleButton);

	quitButton = new GUIButton(_app, { 125, 220 }, 64, 32, "Assets/textures/UI/Exit.png");
	guiList.add(quitButton);

	// Settings Screen
	fullScreenCheck = new GUICheckbox(_app, { 50,220 }, 32, 32, "Assets/textures/UI/Checkbox.png");
	settingsGUI.add(fullScreenCheck);

	vSyncCheck = new GUICheckbox(_app, { 50,264 }, 32, 32, "Assets/textures/UI/Checkbox.png");
	settingsGUI.add(vSyncCheck);

	musicSlider = new GUISlider(_app, { 55, 110}, 128, 16, "Assets/textures/UI/Slider.png");
	musicSlider->CreateGUIBtn(new GUIButton(_app, { 55, 110 }, 10, 16, "Assets/textures/UI/Slider_Btn.png"));
	settingsGUI.add(musicSlider);

	sfxSlider = new GUISlider(_app, { 55, 170 }, 128, 16, "Assets/textures/UI/Slider.png");
	sfxSlider->CreateGUIBtn(new GUIButton(_app, { 55, 170 }, 10, 16, "Assets/textures/UI/Slider_Btn.png"));
	settingsGUI.add(sfxSlider);

	backToPause = new GUIButton(_app, { 0, 0 }, 32, 32, "Assets/textures/UI/ReturnBtn.png");
	settingsGUI.add(backToPause);

	InitializeSettings();

	backgroundTexture = _app->textures->Load("Assets/textures/UI/Panel_pause.png");
	settingsBackgroundTexture = _app->textures->Load("Assets/textures/UI/Panel_setting.png");
}

void PanelPause::CheckInteractions()
{
	switch (currentScreen)
	{
	case 0:
		if (resumeButton->doAction)
		{
			TogglePause();
			resumeButton->doAction = false;
		}
		if (settingsButton->doAction)
		{
			currentScreen = 1;
			settingsButton->doAction = false;
		}
		if (backToTitleButton->doAction)
		{
			_app->scene->ChangeCurrentSceneRequest(0);
			backToTitleButton->doAction = false;
		}
		if (quitButton->doAction)
		{
			_app->ExitGame();
		}
		break;
	case 1:
		if (backToPause->doAction)
		{
			currentScreen = 0;
			backToPause->doAction = false;
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
	}
}

void PanelPause::Update()
{
	if (!isActive) return;

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

void PanelPause::PostUpdate()
{
	if (!isActive) return;

	if (currentScreen == 0) _app->renderer->AddTextureRenderQueue(backgroundTexture, { 0,0 }, { 0,0,0,0 }, 1, 3, 5, 0, SDL_FLIP_NONE, 0);
	if (currentScreen == 1) _app->renderer->AddTextureRenderQueue(settingsBackgroundTexture, { 0,0 }, { 0,0,0,0 }, 1, 3, 5, 0, SDL_FLIP_NONE, 0);

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

void PanelPause::CleanUp()
{
	SaveSettings();
	guiList.clearPtr();
	settingsGUI.clearPtr();
}

void PanelPause::TogglePause()
{
	isActive = !isActive;
	_app->debug->debugPause = isActive;
	currentScreen = 0;
}

void PanelPause::InitializeSettings()
{
	pugi::xml_node n = _app->saveF.child("game_state").child("settings");

	fullScreenCheck->ChangeState(n.attribute("fullScreen").as_bool(false));
	vSyncCheck->ChangeState(n.attribute("vSync").as_bool(true));
	musicSlider->SetValue(n.attribute("music").as_float(0.5));
	sfxSlider->SetValue(n.attribute("sfx").as_float(0.5));
}

void PanelPause::SaveSettings()
{
	pugi::xml_node n = _app->saveF.child("game_state").child("settings");

	n.attribute("fullScreen") = fullScreenCheck->isActive;
	n.attribute("vSync") = vSyncCheck->isActive;
	n.attribute("music") = musicSlider->GetValue();
	n.attribute("sfx") = sfxSlider->GetValue();

	_app->saveF.save_file(SAVE_STATE_FILENAME);
}

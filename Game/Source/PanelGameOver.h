#pragma once
#include "GUIPanel.h"

class GUIButton;

class PanelGameOver : public GUIPanel
{
private:

	GUIButton* retryButton = nullptr;
	GUIButton* mainMenuButton = nullptr;
	GUIButton* quitButton = nullptr;

public:

	PanelGameOver(Application* app);

	void CheckInteractions() override;

	void Update() override;

	void PostUpdate() override;

	void CleanUp() override;
};


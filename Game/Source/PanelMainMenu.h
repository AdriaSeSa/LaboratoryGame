#pragma once
#include "GUIPanel.h"

class GUIButton;
class GUICheckbox;
class GUISlider;
class SceneMainMenu;

class PanelMainMenu : public GUIPanel
{
private:

    GUIButton* startButton = nullptr;
    GUIButton* settingsButton = nullptr;
    GUIButton* quitButton = nullptr;
    GUIButton* level1Button = nullptr;
    GUIButton* level2Button = nullptr;
    GUIButton* backToMainMenu1 = nullptr;
    GUIButton* backToMainMenu2 = nullptr;

    GUICheckbox* fullScreenCheck = nullptr;
    GUICheckbox* vSyncCheck = nullptr;

    GUISlider* musicSlider = nullptr;
    GUISlider* sfxSlider = nullptr;

    List<GUI*> settingsGUI;
    List<GUI*> selectLevelGUI;

    SceneMainMenu* scene = nullptr;

    int currentScreen = 0;

public:

    PanelMainMenu(Application* app, SceneMainMenu* scene);

    void CheckInteractions() override;

    void Update() override;

    void PostUpdate() override;

    void CleanUp() override;
private:

    void InitializeSettings();

    void SaveSettings();
};


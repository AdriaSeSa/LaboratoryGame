#pragma once
#include "GUIPanel.h"
#include "SDL/include/SDL.h"

class GUIButton;
class GUICheckbox;
class GUISlider;
class SceneMainMenu;

class PanelMainMenu : public GUIPanel
{
private:

    GUIButton* startButton = nullptr;
    GUIButton* continueButton = nullptr;
    GUIButton* settingsButton = nullptr;
    GUIButton* quitButton = nullptr;
    GUIButton* creditsButton = nullptr;

    GUIButton* backToMainMenu1 = nullptr;
    GUIButton* backToMainMenu2 = nullptr;
    GUIButton* backToMainMenu3 = nullptr;

    GUIButton* level1Button = nullptr;
    GUIButton* level2Button = nullptr;
    GUIButton* levelSecretButton = nullptr;

    GUICheckbox* fullScreenCheck = nullptr;
    GUICheckbox* vSyncCheck = nullptr;

    GUISlider* musicSlider = nullptr;
    GUISlider* sfxSlider = nullptr;

    List<GUI*> settingsGUI;
    List<GUI*> selectLevelGUI;
    List<GUI*> creditsGUI;

    SceneMainMenu* scene = nullptr;

    SDL_Texture* settingsBackgroundTexture = nullptr;
    SDL_Texture* creditsBackgroundTexture = nullptr;

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

    bool CheckForSavedFile();
};


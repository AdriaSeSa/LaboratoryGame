#pragma once
#include "GUIPanel.h"
#include "SDL/include/SDL.h"

class GUIButton;
class GUICheckbox;
class GUISlider;

class PanelPause : public GUIPanel
{
private:
    GUIButton* resumeButton = nullptr;
    GUIButton* settingsButton = nullptr;
    GUIButton* quitButton = nullptr;
    GUIButton* backToTitleButton = nullptr;
    GUIButton* backToPause = nullptr;

    GUICheckbox* fullScreenCheck = nullptr;
    GUICheckbox* vSyncCheck = nullptr;

    GUISlider* musicSlider = nullptr;
    GUISlider* sfxSlider = nullptr; 

    List<GUI*> settingsGUI;

    SDL_Texture* backgroundTexture = nullptr;
    SDL_Texture* settingsBackgroundTexture = nullptr;

    int currentScreen = 0;

    bool isActive = false;
public:

    PanelPause(Application* app);

    void CheckInteractions() override;

    void Update() override;

    void PostUpdate() override;

    void CleanUp() override;

    void TogglePause();
private:

    void InitializeSettings();

    void SaveSettings();
};


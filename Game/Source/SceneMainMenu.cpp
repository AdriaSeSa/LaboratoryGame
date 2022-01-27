#include "SceneMainMenu.h"
#include "BackGround.h"
#include "GUIButton.h"
#include "GUISlider.h"
#include "GUICheckbox.h"
#include "PanelMainMenu.h"

SceneMainMenu::SceneMainMenu(Application* app) : Scene(app)
{
    ID = 0;
}

bool SceneMainMenu::Start()
{
    // Reset logo position
    logoY = -100;

    //GUIButton* buttonTest = new GUIButton(_app, { 100,100 }, 8, 16, "Assets/textures/UI/Slider_BtnTest.png");

    //GUISlider* sliderTest = new GUISlider(_app, { 100,100 }, 128, 16, "Assets/textures/UI/SliderTest.png");
    //sliderTest->CreateGUIBtn(new GUIButton(_app, { 100,100 }, 8, 20, "Assets/textures/UI/Slider_BtnTest.png"));

    //GUICheckbox* checkboxTest = new GUICheckbox(_app, { 100, 200 }, 48, 48, "Assets/textures/UI/CheckboxTest.png");

    //guis.add(sliderTest);

    //guis.add(checkboxTest);

    PanelMainMenu* panelMM = new PanelMainMenu(_app, this);
    panels.add(panelMM);

    InitTextures();

    bg = new BackGround("menuBackGround", "MenuBG", _app);
    gameObjects.add(bg);

    for (int i = 0; i < gameObjects.count(); i++)
    {
        if (gameObjects[i] != nullptr)
            gameObjects[i]->Start();
    }

    // Set screen to MAIN_MENU
    ChangeScreen(0);

    return true;
}

void SceneMainMenu::InitTextures()
{
    // Main Menu
    RenderObject l;
    l.texture = _app->textures->Load("Assets/textures/Menu/LogoAnim.png");
    l.destRect.x = 100;
    l.destRect.y = 100;
    l.name = "logo";
    for (int i = 0; i < 4; i++)
    {
        logoAnim.PushBack({ 128 * i, 0, 128, 64 });
    }
    logoAnim.loop = true;
    logoAnim.speed = 0.1f;
    logoAnim.hasIdle = false;

    RenderObject m;
    m.texture = _app->textures->Load("Assets/textures/Menu/MenuOptions.png");
    m.destRect.x = 0;
    m.destRect.y = 0;
    m.scale = 0.5f;

    mainMenuTextures.add(l);
    mainMenuTextures.add(m);

    // Select Level

    RenderObject sl;
    sl.texture = _app->textures->Load("Assets/textures/Menu/SelectLevel.png");
    sl.destRect.x = 0;
    sl.destRect.y = 0;
    sl.scale = 0.5f;

    RenderObject l1;
    l1.texture = _app->textures->Load("Assets/textures/Menu/Levels.png");
    l1.destRect.x = 69;
    l1.destRect.y = 135;
    l1.section = { 64, 0, 32, 32 };

    RenderObject l2;
    l2.texture = _app->textures->Load("Assets/textures/Menu/Levels.png");
    l2.destRect.x = 210;
    l2.destRect.y = 135;
    l2.section = { 96, 0, 32, 32 };

    selectLevelTextures.add(sl);
    selectLevelTextures.add(l1);
    selectLevelTextures.add(l2);
}

bool SceneMainMenu::Update()
{
    if (exit) return false;

    Scene::Update();

    for (int i = 0; i < gameObjects.count(); i++)
    {
        if (gameObjects[i] != nullptr)
        gameObjects[i]->Update();
    }

    for (int i = 0; i < panels.count(); i++)
    {
        panels[i]->Update();
    }

    if (bg->movementY == -256) bg->movementY = 0;

    bg->movementY--;

    // Main Menu Animation
    if (currentScreen == 0)
    {
        logoY = logoY == 58 ? logoY : logoY + 2;
    }

    return true;
}

bool SceneMainMenu::PostUpdate()
{
    Scene::PostUpdate();
    for (int i = 0; i < gameObjects.count(); i++)
    {
        if (gameObjects[i] != nullptr)
            gameObjects[i]->PostUpdate();
    }

    for (int i = 0; i < panels.count(); i++)
    {
        panels[i]->PostUpdate();
    }
       
    for (int i = 0; i < currentTextures.count(); i++)
    {
        // Make currentTextures a List<RenderObjects> and use that to render
        if (currentTextures[i].name == "logo")
        {
            logoAnim.Update();
            _app->renderer->AddTextureRenderQueue(currentTextures[i].texture, { currentTextures[i].destRect.x, logoY }, logoAnim.GetCurrentFrame(), 1, 2, 1);
        }
        else
        {
            _app->renderer->AddTextureRenderQueue(currentTextures[i].texture, { currentTextures[i].destRect.x, currentTextures[i].destRect.y }, currentTextures[i].section, 
              currentTextures[i].scale, 2, 1);
        }
    }

    return true;
}

void SceneMainMenu::ChangeScreen(int screen)
{
    currentScreen = screen;
    currentTextures.clear();
    switch (screen)
    {
    // Main Menu
    case 0:
        for (int i = 0; i < mainMenuTextures.count(); i++)
        {
            currentTextures.add(mainMenuTextures[i]);
        }
        break;
    // Select Level   
    case 1:
        for (int i = 0; i <selectLevelTextures.count(); i++)
        {
            currentTextures.add(selectLevelTextures[i]);
        }
        break;

    }
}

bool SceneMainMenu::CleanUp()
{
    Scene::CleanUp();

    currentTextures.clear();
    mainMenuTextures.clear();
    selectLevelTextures.clear();
   
    for (int i = 0; i < panels.count(); i++)
    {
        panels[i]->CleanUp();
    }

    panels.clearPtr();

    return true;
}
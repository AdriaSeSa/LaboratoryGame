#include "SceneMainMenu.h"
#include "BackGround.h"

SceneMainMenu::SceneMainMenu(Application* app) : Scene(app)
{
    ID = 0;
}

bool SceneMainMenu::Start()
{
    // Reset logo position
    logoY = -100;
    
    arrowAnimOffset = 0;

    arrowAnimLeft = false;

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
    arrow = _app->textures->Load("Assets/textures/Menu/Arrow.png");
    arrowSection = { 0,0,32,32 };
    sArrow = _app->textures->Load("Assets/textures/Menu/Levels.png");
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

void SceneMainMenu::MoveArrow()
{
    if (_app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || _app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || _app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
    {
        _app->audio->PlayFx(SFX::BLIP_SELECT);
        // If we are at the last position of arrowPositions, we go back to the beginning. if not, we advance one position.
        currentArrowPos == arrowPositions.count()-1 ? currentArrowPos = 0 : currentArrowPos++;
    }
    if (_app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || _app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || _app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
    {
        _app->audio->PlayFx(SFX::BLIP_SELECT);
        // If we are at the first position of arrowPositions, we go forward to the end. if not, we go back one position.
        currentArrowPos == 0 ? currentArrowPos = arrowPositions.count()-1 : currentArrowPos--;
    }
    if (_app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || _app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
    {
        _app->audio->PlayFx(SFX::SELECT);
        SelectOption();
        //arrowSection = { 32,0,32,32 };
    }
}

bool SceneMainMenu::Update()
{
    if (exit) return false;

    Scene::Update();

    arrowAnimLeft = arrowAnimOffset == 20 ? true : arrowAnimOffset == 0 ? false : arrowAnimLeft;

    arrowAnimLeft ? arrowAnimOffset-- : arrowAnimOffset++;

    for (int i = 0; i < gameObjects.count(); i++)
    {
        if (gameObjects[i] != nullptr)
        gameObjects[i]->Update();
    }

    if (bg->movementY == -256) bg->movementY = 0;

    bg->movementY--;

    // Main Menu Animation
    if (currentScreen == 0)
    {
        logoY = logoY == 58 ? logoY : logoY + 2;
    }

    MoveArrow();

    return true;
}

bool SceneMainMenu::PostUpdate()
{
    for (int i = 0; i < gameObjects.count(); i++)
    {
        if (gameObjects[i] != nullptr)
            gameObjects[i]->PostUpdate();
    }
    if (currentScreen == 0) _app->renderer->AddTextureRenderQueue(arrow, arrowPositions[currentArrowPos] + iPoint(arrowAnimOffset/4, 0), arrowSection, 1, 2, 1);
    if (currentScreen == 1)
    {
        if (currentArrowPos < 2) _app->renderer->AddTextureRenderQueue(sArrow, arrowPositions[currentArrowPos], { 0,0,32,32 }, 1, 2, 2);
        else  _app->renderer->AddTextureRenderQueue(arrow, arrowPositions[currentArrowPos] + iPoint(arrowAnimOffset/4, 0), arrowSection, 1, 2, 1);
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
    arrowPositions.clear();
    currentArrowPos = 0;
    currentScreen = screen;
    currentTextures.clear();
    switch (screen)
    {
    // Main Menu
    case 0:
        arrowPositions.add({ 69, 190 });
        arrowPositions.add({ 69, 260 });
        for (int i = 0; i < mainMenuTextures.count(); i++)
        {
            currentTextures.add(mainMenuTextures[i]);
        }
        break;
    // Select Level   
    case 1:
        arrowPositions.add({ 69, 135 });
        arrowPositions.add({ 210, 135 });
        arrowPositions.add({ 95, 180 });
        for (int i = 0; i <selectLevelTextures.count(); i++)
        {
            currentTextures.add(selectLevelTextures[i]);
        }
        break;

    }
}

void SceneMainMenu::SelectOption()
{
    switch (currentScreen)
    {
    case 0:
        ScreenOptions0();
        break;
    case 1:
        ScreenOptions1();
        break;
    }
}

void SceneMainMenu::ScreenOptions0()
{
    switch (currentArrowPos)
    {
    case 0:
        ChangeScreen(1);
        break;
    case 1:
        exit = true;
        break;
    }
}

void SceneMainMenu::ScreenOptions1()
{
    switch (currentArrowPos)
    {
    case 0:
        _app->scene->ChangeCurrentScene(2);
        break;
    case 1:
        _app->scene->ChangeCurrentScene(3);
        break;
    case 2:
        ChangeScreen(0);
        break;
    }
}

void SceneMainMenu::ScreenOptions2()
{
}

bool SceneMainMenu::CleanUp()
{
    Scene::CleanUp();

    currentTextures.clear();
    mainMenuTextures.clear();
    selectLevelTextures.clear();
    arrowPositions.clear();

    return true;
}
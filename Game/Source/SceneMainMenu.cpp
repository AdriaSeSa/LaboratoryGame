#include "SceneMainMenu.h"
#include "BackGround.h"

SceneMainMenu::SceneMainMenu(Application* app) : Scene(app)
{
    ID = 0;
}

bool SceneMainMenu::Start(bool reset)
{
    bg = new BackGround("menuBackGround", "MenuBG", _app);

    // Reset logo position
    logoY = -100;
    
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
    mainMenu = _app->textures->Load("Assets/textures/Menu/MenuOptions.png");
    logo = _app->textures->Load("Assets/textures/Menu/Logo.png");
   // arrow = _app->textures->Load("Assets/textures/Menu/Arrow.png");

    mainMenuTextures.add(mainMenu);
    mainMenuTextures.add(logo);
}

void SceneMainMenu::MoveArrow()
{
    if (_app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || _app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || _app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
    {
        // If we are at the last position of arrowPositions, we go back to the beginning. if not, we advance one position.
        currentArrowPos == arrowPositions.count() ? currentArrowPos = 0 : currentArrowPos++;
    }
    if (_app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || _app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || _app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
    {
        // If we are at the first position of arrowPositions, we go forward to the end. if not, we go back one position.
        currentArrowPos == 0 ? currentArrowPos = arrowPositions.count() : currentArrowPos--;
    }

    if (_app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || _app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
    {
        SelectOption();
    }
}

bool SceneMainMenu::Update()
{
    if (exit) return false;

    for (int i = 0; i < gameObjects.count(); i++)
    {
        if (gameObjects[i] != nullptr)
        gameObjects[i]->Update();
    }

    // Main Menu Animation
    if (currentScreen == 0)
    {
        if (bg->movementY == -256) bg->movementY = 0;

        bg->movementY--;

        logoY = logoY == 58 ? logoY : logoY + 1;
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

    for (int i = 0; i < currentTextures.count(); i++)
    {
        // Make currentTextures a List<RenderObjects> and use that to render

        //_app->renderer->AddTextureRenderQueue()
    }

    return true;
}

void SceneMainMenu::ChangeScreen(int screen)
{
    arrowPositions.clear();
    currentScreen = screen;
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

        currentTextures = selectLevelTextures;
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
        _app->scene->ChangeCurrentScene(2, true);
        break;
    case 1:
        exit = true;
        break;
    case 2:
        break;
    }
}

void SceneMainMenu::ScreenOptions1()
{
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

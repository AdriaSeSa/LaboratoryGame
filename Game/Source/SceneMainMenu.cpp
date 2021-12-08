#include "SceneMainMenu.h"
#include "BackGround.h"

SceneMainMenu::SceneMainMenu(Application* app) : Scene(app)
{
    ID = 0;
}

bool SceneMainMenu::Start(bool reset)
{
    mainMenu = _app->textures->Load("Assets/textures/Menu/MenuOptions.png");
    logo = _app->textures->Load("Assets/textures/Menu/Logo.png");
    arrow = _app->textures->Load("Assets/textures/Menu/Arrow.png");

    bg = new BackGround("menuBackGround", "MenuBG", _app);

    // Reset logo position
    logoY = -100;

    // Reset arrow position
    arrowPos = { 69, 190 };

    gameObjects.add(bg);

    for (int i = 0; i < gameObjects.count(); i++)
    {
        if (gameObjects[i] != nullptr)
            gameObjects[i]->Start();
    }

    return true;
}

bool SceneMainMenu::Update()
{
    for (int i = 0; i < gameObjects.count(); i++)
    {
        if (gameObjects[i] != nullptr)
        gameObjects[i]->Update();
    }

    if (bg->movementY == -256) bg->movementY = 0;

    bg->movementY--;

    logoY = logoY == 58 ? logoY : logoY + 1;

    if (_app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || _app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
    {
        arrowPos.y = arrowPos.y == 260 ? 190 : 260;
    }
    if (_app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || _app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
    {
        arrowPos.y = arrowPos.y == 190 ? 260 : 190;
    }

    if (_app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || _app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
    {
        if (arrowPos.y == 190)
        {
            _app->scene->ChangeCurrentScene(2, true);
        }
        else
        {
            return false;
        }
    }

    return true;
}

bool SceneMainMenu::PostUpdate()
{
    for (int i = 0; i < gameObjects.count(); i++)
    {
        if (gameObjects[i] != nullptr)
            gameObjects[i]->PostUpdate();
    }
    _app->renderer->AddTextureRenderQueue(mainMenu, { 0,0, }, { 0,0,0,0 }, 0.5f);
    _app->renderer->AddTextureRenderQueue(logo, { 96,logoY, });
    _app->renderer->AddTextureRenderQueue(arrow, arrowPos, { 0,0,0,0 }, 1, 1);

    return true;
}
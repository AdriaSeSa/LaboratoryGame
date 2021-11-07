#include "SceneMainMenu.h"
#include "BackGround.h"

SceneMainMenu::SceneMainMenu(Application* app) : Scene(app)
{

}


bool SceneMainMenu::Start()
{
    SDL_Texture* mainMenu = _app->textures->Load("Assets/textures/Menu/MenuOptions.png");
    SDL_Texture* logo = _app->textures->Load("Assets/textures/Menu/Logo.png");
    SDL_Texture* arrow = _app->textures->Load("Assets/textures/Menu/Arrow.png");
    bg = new BackGround("menuBackGround", "MenuBG", _app);

    arrowPos = { 69, 190 };

    sceneTextures.add(mainMenu);
    sceneTextures.add(logo);
    sceneTextures.add(arrow);

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
            _app->scene->ChangeCurrentScene(0, 0);
        }
        else
        {
            return false;
        }
        //SelectDifficulty();
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
    _app->renderer->AddTextureRenderQueue(sceneTextures[0], { 0,0, });
    _app->renderer->AddTextureRenderQueue(sceneTextures[1], { 96,logoY, });
    _app->renderer->AddTextureRenderQueue(sceneTextures[2], arrowPos, { 0,0,0,0 }, 1, 1);

    return true;
}

void SceneMainMenu::SelectDifficulty()
{
   

}

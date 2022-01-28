#include "SceneGameOver.h"
#include "BackGround.h"
#include "PanelGameOver.h"

SceneGameOver::SceneGameOver(Application* app) : Scene(app)
{
    ID = 1;
}

bool SceneGameOver::Start()
{
    mainMenu = _app->textures->Load("Assets/textures/Menu/GameOverOptions.png", false);
    arrow = _app->textures->Load("Assets/textures/Menu/ArrowGO.png", false);
    bg = new BackGround("menuBackGround", "MenuBG", _app);

    arrowPos = { 20, 190 };

    gameObjects.add(bg);

    //_app->SaveGameRequest();

    for (int i = 0; i < gameObjects.count(); i++)
    {
        if (gameObjects[i] != nullptr)
            gameObjects[i]->Start();
    }

    panel = new PanelGameOver(_app);

    return true;
}

bool SceneGameOver::Update()
{
    Scene::Update();

    if (panel != nullptr)
    {
        panel->Update();
    }

    for (int i = 0; i < gameObjects.count(); i++)
    {
        if (gameObjects[i] != nullptr)
            gameObjects[i]->Update();
    }

    if (bg->movementY == -256) bg->movementY = 0;

    bg->movementY--;

    return true;
}

bool SceneGameOver::PostUpdate()
{
    Scene::PostUpdate();

    if (panel != nullptr)
    {
        panel->PostUpdate();
    }

    for (int i = 0; i < gameObjects.count(); i++)
    {
        if (gameObjects[i] != nullptr)
            gameObjects[i]->PostUpdate();
    }
    _app->renderer->AddTextureRenderQueue(mainMenu, { 0,0, });

    return true;
}

bool SceneGameOver::CleanUp()
{
    if (panel != nullptr)
    {
        panel->CleanUp();
        RELEASE(panel);
    }
    return true;
}


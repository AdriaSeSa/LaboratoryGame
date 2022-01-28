#include "SceneGameOver.h"
#include "BackGround.h"

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

    return true;
}

bool SceneGameOver::Update()
{
    Scene::Update();

    for (int i = 0; i < gameObjects.count(); i++)
    {
        if (gameObjects[i] != nullptr)
            gameObjects[i]->Update();
    }

    if (bg->movementY == -256) bg->movementY = 0;

    bg->movementY--;

    if (_app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || _app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
    {
        _app->audio->PlayFx(SFX::BLIP_SELECT);
        arrowPos.y = arrowPos.y == 250 ? 190 : 250;
    }
    if (_app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || _app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
    {
        _app->audio->PlayFx(SFX::BLIP_SELECT);
        arrowPos.y = arrowPos.y == 190 ? 250 : 190;
    }

    if (_app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || _app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
    {
        _app->audio->PlayFx(SFX::SELECT);
        if (arrowPos.y == 190)
        {
            _app->scene->ChangeCurrentSceneRequest(_app->scene->lastLevel);
        }
        else
        {
            _app->scene->ChangeCurrentSceneRequest(0);
        }
        //SelectDifficulty();
    }

    return true;
}

bool SceneGameOver::PostUpdate()
{
    Scene::PostUpdate();
    for (int i = 0; i < gameObjects.count(); i++)
    {
        if (gameObjects[i] != nullptr)
            gameObjects[i]->PostUpdate();
    }
    _app->renderer->AddTextureRenderQueue(mainMenu, { 0,0, });
    _app->renderer->AddTextureRenderQueue(arrow, arrowPos, { 0,0,0,0 }, 1, 1);

    return true;
}

//void SceneGameOver::SetSaveData()
//{
//    playerX = playerStartPos.x;
//    playerY = playerStartPos.y;
//
//    _app->scene->ResetPlayerSettings();
//}

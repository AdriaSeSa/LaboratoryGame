#include "SceneGame.h"
#include "ModuleScene.h"
#include "Player.h"
#include "CheckPoint.h"

SceneGame::SceneGame(Application* app, string name) : Scene(app, name)
{
}

void SceneGame::SaveGameFile()
{
	pugi::xml_node n = _app->saveF.child("game_state").child("scene");

	n.child("player").attribute("lifes") = PlayerSettings::Instance(_app)->playerLifes;
	n.child("player").attribute("score") = PlayerSettings::Instance(_app)->playerScore;

	n.child(name.c_str()).child("player").attribute("x") = player->GetPosition().x;
	n.child(name.c_str()).child("player").attribute("y") = player->GetPosition().y;

	pugi::xml_node node = _app->saveF.child("game_state").child("debug").child("lastState");

	int currentSceneIndex = (int)_app->scene->currentSceneState;

	node.attribute("isSaved") = "true";
	node.attribute("scene") = currentSceneIndex;

	_app->SaveGameRequest();
}

void SceneGame::ResetGameFile()
{
	_app->scene->ResetPlayerSettings();

	pugi::xml_node n = _app->saveF.child("game_state").child("scene");

	n.child(name.c_str()).child("checkPoint").attribute("isActive") = "false";

	n.child("player").attribute("lifes") = 2;
	n.child("player").attribute("score") = 0;

	n.child(name.c_str()).child("player").attribute("x") = playerStartPos.x;
	n.child(name.c_str()).child("player").attribute("y") = playerStartPos.y;

	pugi::xml_node node = _app->saveF.child("game_state").child("debug").child("lastState");

	node.attribute("isSaved") = "false";
	node.attribute("scene") = "0";

	_app->SaveGameRequest();
}

void SceneGame::LoadGameFile()
{
	pugi::xml_node n = _app->saveF.child("game_state").child("scene");

	if (player != nullptr) player->SetPosition({ n.child(name.c_str()).child("player").attribute("x").as_int(),n.child(name.c_str()).child("player").attribute("y").as_int() });

	if (checkPoint != nullptr) checkPoint->isActive = _app->scene->playerSettings->reachedCheckPoint;

	if (_app->scene->playerSettings->playerLifes != 0) return;

	_app->scene->playerSettings->playerLifes = n.child("player").attribute("lifes").as_int();
	_app->scene->playerSettings->playerScore = n.child("player").attribute("score").as_int();
}

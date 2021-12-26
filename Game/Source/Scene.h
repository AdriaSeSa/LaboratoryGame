#ifndef _SCENE_H_
#define _SCENE_H_

#include "GameObject.h"
#include "List.h"

class Scene
{
protected:

	int ID = 0;

	int platformLenght = 2;

	Application* _app = nullptr;

	List<GameObject*> gameObjects;

	//List<SDL_Texture*> sceneTextures;

public:
	// Constructor
	Scene(Application* app, string name = "null");

	// Destructor
	~Scene();

	virtual bool InitScene();

	virtual bool Start(bool reset = false);

	virtual bool PreUpdate();

	virtual bool Update();

	virtual bool PostUpdate();

	virtual bool CleanUp();

	void DestroyGameObject(GameObject* gameObject);

	virtual void SetSaveData();

	virtual void LoadSaveData(pugi::xml_node save);

	int getID()
	{
		return ID;
	}

	int lastID = 0;

	int playerX = 0;
	int playerY = 0;

	int isWin = false;

	std::string name = "null";
};

#endif //_SCENE_H_
#pragma once
#include "Module.h"
#include "SDL\include\SDL.h"
#include "List.h"
#include <map>

struct TexturePath
{
	string path = "";
	long index = 0;
	int direccion;
};

class ModuleTextures : public Module
{
public:
	ModuleTextures(Application* app, bool start_enabled = true);
	~ModuleTextures();

	bool Init(pugi::xml_node&);
	bool CleanUp();
	//bool CleanUpTextures();

	//SDL_Texture* Load(std::string path);

	SDL_Texture* Load(std::string path, bool isName = false);

	void Unload(SDL_Texture* texture);

public:
	List<SDL_Texture*> textures;

private:
	List<TexturePath> texturePath;

	//std::map<std::string, int> texturePath = { {"", 0} };
};
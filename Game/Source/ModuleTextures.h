#pragma once
#include "Module.h"
#include "SDL\include\SDL.h"
#include "List.h"
#include <map>
#include <string>


class ModuleTextures : public Module
{
public:
	ModuleTextures(Application* app, bool start_enabled = true);
	~ModuleTextures();

	bool Init();
	bool CleanUp();
	bool CleanUpTextures();

	SDL_Texture* Load(std::string path);
	void Unload(SDL_Texture* texture);

public:
	p2List<SDL_Texture*> textures;

private:
	std::map<std::string, SDL_Texture*> texturePath;
};
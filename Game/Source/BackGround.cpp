#include "BackGround.h"

BackGround::BackGround(std::string name, std::string tag, Application* app) :GameObject(name, tag, app)
{
	renderObjects[0].texture = app->textures->Load("Assets/textures/Background/Blue.png");
	renderObjects[0].renderRect.w = renderObjects[0].renderRect.h = 64;

	SDL_GetWindowSize(app->window->window, &WindowWidth, &WindowHeight);
}

void BackGround::PostUpdate()
{
	for (int i = 0; i < WindowHeight; i += 64)
	{
		for (int j = 0; j < WindowWidth; j += 64)
		{
			renderObjects[0].renderRect.x = j;
			renderObjects[0].renderRect.y = i;

			_app->renderer->AddTextureRenderQueue(renderObjects[0]);
		}
	}
}

#include "BackGround.h"

BackGround::BackGround(std::string name, std::string tag, Application* app) :GameObject(name, tag, app)
{
	// Init renderObject
	InitRenderObjectWithXml();

	// Get windows w&h
	SDL_GetWindowSize(app->window->window, &WindowWidth, &WindowHeight);
}

void BackGround::PostUpdate()
{
	for (int i = 0; i < WindowHeight; i += renderObjects[0].renderRect.h)
	{
		for (int j = 0; j < WindowWidth; j += renderObjects[0].renderRect.w)
		{
			renderObjects[0].renderRect.x = j;
			renderObjects[0].renderRect.y = i;

			_app->renderer->AddTextureRenderQueue(renderObjects[0]);
		}
	}
}

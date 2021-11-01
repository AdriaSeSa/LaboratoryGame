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
	for (int i = 0; i < WindowHeight; i += renderObjects[0].destRect.h)
	{
		for (int j = 0; j < WindowWidth; j += renderObjects[0].destRect.w)
		{
			renderObjects[0].destRect.x = j;
			renderObjects[0].destRect.y = i;

			_app->renderer->AddTextureRenderQueue(renderObjects[0]);
		}
	}
}

#include "App.h"
#include "Window.h"
#include "Render.h"

#include "Defs.h"
#include "Log.h"

#define VSYNC true

Render::Render() : Module()
{
	name.Create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;

	layers.resize(4);
}

// Destructor
Render::~Render()
{}

// Called before render is available
bool Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;

	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(app->win->window, -1, flags);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = app->win->screenSurface->w;
		camera.h = app->win->screenSurface->h;
		camera.x = 0;
		camera.y = 0;
	}

	return ret;
}

// Called before the first frame
bool Render::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);
	return true;
}

// Called each loop iteration
bool Render::PreUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool Render::Update(float dt)
{
	return true;
}

bool Render::PostUpdate()
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);

	// Sorting layers
	for (int i = 0; i < layers.size(); i++)
	{
		SortRenderObjects(layers[i]);
	}

	//Draw
	for (int i = 0; i < 3; i++)
	{
		for each (auto renderObject in layers[i])
		{
			//SDL_SetTextureAlphaMod(renderObject.texture, 100);

			if (SDL_RenderCopyEx(renderer, renderObject.texture, &renderObject.section, &renderObject.renderRect, renderObject.rotation, NULL, renderObject.flip) != 0)
			{
				LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
			}
		}
	}

	// Draw Rects
	for (int i = 0; i < rects.size(); i++)
	{
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, rects[i].color.r, rects[i].color.g, rects[i].color.b, rects[i].color.a);

		SDL_RenderFillRect(renderer, &rects[i].rect);
	}

	// Draw Special Layer
	for each (auto renderObject in layers[3])
	{
		if (SDL_RenderCopyEx(renderer, renderObject.texture, &renderObject.section, &renderObject.renderRect, renderObject.rotation, NULL, renderObject.flip) != 0)
		{
			LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		}
	}

	SDL_RenderPresent(renderer);

	for (int i = 0; i < 4; i++)
	{
		layers[i].clear();
	}

	rects.clear();

	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool Render::CleanUp()
{
	LOG("Destroying SDL render");
	if (renderer != NULL)
	{
		SDL_DestroyRenderer(renderer);
	}
	return true;
}

// L02: DONE 6: Implement a method to load the state, for now load camera's x and y
// Load Game State
bool Render::LoadState(pugi::xml_node& data)
{
	camera.x = data.child("camera").attribute("x").as_int();
	camera.y = data.child("camera").attribute("y").as_int();

	return true;
}

// L02: DONE 8: Create a method to save the state of the renderer
// Save Game State
bool Render::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;

	return true;
}

void Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

void Render::AddTextureRenderQueue(SDL_Texture* texture, iPoint pos, SDL_Rect* section, float scale, int layer, float orderInlayer, float rotation, SDL_RendererFlip flip, float speed)
{
	RenderObject renderObject;
	uint screenSize = app->win->GetScale();

	speed = defaultSpeed;

	renderObject.texture = texture;
	renderObject.rotation = rotation;
	renderObject.section = *section;
	renderObject.orderInLayer = orderInlayer;

	if (layer == 2 || layer == 3) speed = 0;	//If texture in UI layer, it moves alongside the camera. Therefor, speed = 0;

	renderObject.renderRect.x = (int)(-camera.x * speed) + pos.x * screenSize;
	renderObject.renderRect.y = (int)(-camera.y * speed) + pos.y * screenSize;

	if (section != nullptr)
	{
		renderObject.renderRect.w = section->w;
		renderObject.renderRect.h = section->h;
	}
	else
	{
		// Collect the texture size into rect.w and rect.h variables
		SDL_QueryTexture(texture, nullptr, nullptr, &renderObject.renderRect.w, &renderObject.renderRect.h);
	}

	renderObject.renderRect.w *= scale;
	renderObject.renderRect.h *= scale;

	renderObject.flip = flip;

	layers[layer].push_back(renderObject);
}

void Render::AddTextureRenderQueue(RenderObject object)
{
	object.speed = defaultSpeed;
	uint screenSize = app->win->GetScale();

	object.renderRect.x = (int)(-camera.x * object.speed) + object.renderRect.x * screenSize;
	object.renderRect.y = (int)(-camera.y * object.speed) + object.renderRect.y * screenSize;

	if (object.section.w != 0 && object.section.h != 0)
	{
		object.renderRect.w = object.section.w;
		object.renderRect.h = object.section.h;
	}
	else
	{
		// Collect the texture size into rect.w and rect.h variables
		SDL_QueryTexture(object.texture, nullptr, nullptr, &object.renderRect.w, &object.renderRect.h);
	}

	object.renderRect.w *= object.scale;
	object.renderRect.h *= object.scale;

	layers[object.layer].push_back(object);
}

void Render::AddRectRenderQueue(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera)
{
	RenderRect renderR;

	renderR.rect = rect;
	renderR.color = { r,g,b,a };

	rects.push_back(renderR);
}

void Render::SortRenderObjects(vector<RenderObject>& obj)
{
	//sort(obj.begin(), obj.end(), CompareRenderObj);

	int less = 0;
	int objSize = obj.size();

	for (int i = 0; i < objSize; ++i)
	{
		less = i;
		for (int j = i; j < objSize; ++j)
		{
			if (obj[j].orderInLayer < obj[less].orderInLayer)
			{
				swap(obj[j], obj[less]);
			}
		}
	}
}


// Blit to screen
bool Render::DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, double angle, int pivotX, int pivotY) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * scale;
	rect.y = (int)(camera.y * speed) + y * scale;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if(pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}

	if(SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}
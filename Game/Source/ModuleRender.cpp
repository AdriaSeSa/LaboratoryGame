#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"

ModuleRender::ModuleRender(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	renderer = NULL;
	camera.x = camera.y = 0;
	camera.w = SCREEN_WIDTH;
	camera.h = SCREEN_HEIGHT;

	layers.resize(4);
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	bool ret = true;
	Uint32 flags = 0;

	if(VSYNC == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
	}

	renderer = SDL_CreateRenderer(App->window->window, -1, flags);
	
	if(renderer == NULL)
	{
		LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// PreUpdate: clear buffer
UpdateStatus ModuleRender::PreUpdate()
{
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 0);
	SDL_RenderClear(renderer);
	return UPDATE_CONTINUE;
}

// Update: debug camera
UpdateStatus ModuleRender::Update()
{	
	int speed = 3;

	//if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	//	App->renderer->camera.y += speed;

	//if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	//	App->renderer->camera.y -= speed;

	//if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	//	App->renderer->camera.x += speed;

	//if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	//	App->renderer->camera.x -= speed;
	//
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UpdateStatus ModuleRender::PostUpdate()
{
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

			if (SDL_RenderCopyEx(renderer, renderObject.texture, renderObject.section, &renderObject.renderRect, renderObject.rotation, NULL, renderObject.flip) != 0)
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
		if (SDL_RenderCopyEx(renderer, renderObject.texture, renderObject.section, &renderObject.renderRect, renderObject.rotation, NULL, renderObject.flip) != 0)
		{
			LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		}
	}

	if (App->isDebug)
	{
		App->physics->ShapesRender();
	}
	
	SDL_RenderPresent(renderer);

	for (int i = 0; i < 4; i++)
	{
		layers[i].clear();
	}

	rects.clear();

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	if(renderer != NULL)
	{
		SDL_DestroyRenderer(renderer);
	}

	return true;
}

void ModuleRender::AddTextureRenderQueue(SDL_Texture* texture, iPoint pos, SDL_Rect* section, float scale, int layer, float orderInlayer, float rotation, SDL_RendererFlip flip, float speed)
{
	RenderObject renderObject;

	speed = defaultSpeed;

	renderObject.texture = texture;
	renderObject.rotation = rotation;
	renderObject.section = section;
	renderObject.orderInLayer = orderInlayer;

	if (layer == 2 || layer == 3) speed = 0;	//If texture in UI layer, it moves alongside the camera. Therefor, speed = 0;

	renderObject.renderRect.x = (int)(-camera.x * speed) + pos.x * SCREEN_SIZE;
	renderObject.renderRect.y = (int)(-camera.y * speed) + pos.y * SCREEN_SIZE;

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

void ModuleRender::AddTextureRenderQueue(RenderObject object)
{
	object.speed = defaultSpeed;

	object.renderRect.x = (int)(-camera.x * object.speed) + object.renderRect.x * SCREEN_SIZE;
	object.renderRect.y = (int)(-camera.y * object.speed) + object.renderRect.y * SCREEN_SIZE;

	if (object.section != nullptr)
	{
		object.renderRect.w = object.section->w;
		object.renderRect.h = object.section->h;
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

void ModuleRender::AddRectRenderQueue(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera)
{
	RenderRect renderR;

	renderR.rect = rect;
	renderR.color = { r,g,b,a };
	
	rects.push_back(renderR);
}

void ModuleRender::SortRenderObjects(vector<RenderObject>& obj)
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

void ModuleRender::CameraMove(iPoint pos)
{
	camera.x = pos.x + (SCREEN_WIDTH / 2);	//	Camera position = target position

	camera.y = pos.y;
}

#pragma region OBSOLETE

// Blit to screen
bool ModuleRender::Blit(SDL_Texture* texture, int x, int y, float scale, SDL_Rect* section, float speed, double angle, SDL_RendererFlip flip, int pivot_x, int pivot_y)
{
	bool ret = true;
	SDL_Rect rect;
	rect.x = (int) (camera.x * speed) + x * SCREEN_SIZE;
	rect.y = (int) (camera.y * speed) + y * SCREEN_SIZE;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= SCREEN_SIZE;
	rect.h *= SCREEN_SIZE;

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if(pivot_x != INT_MAX && pivot_y != INT_MAX)
	{
		pivot.x = pivot_x;
		pivot.y = pivot_y;
		p = &pivot;
	}

	if(SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(camera.x + rect.x * SCREEN_SIZE);
		rec.y = (int)(camera.y + rect.y * SCREEN_SIZE);
		rec.w *= SCREEN_SIZE;
		rec.h *= SCREEN_SIZE;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);
	
	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * SCREEN_SIZE, camera.y + y1 * SCREEN_SIZE, camera.x + x2 * SCREEN_SIZE, camera.y + y2 * SCREEN_SIZE);
	else
		result = SDL_RenderDrawLine(renderer, x1 * SCREEN_SIZE, y1 * SCREEN_SIZE, x2 * SCREEN_SIZE, y2 * SCREEN_SIZE);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float) M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int) (x + radius * cos( i * factor));
		points[i].y = (int) (y + radius * sin( i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

#pragma endregion
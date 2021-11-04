#include "Application.h"

int cameraSpeed = 1;

ModuleRender::ModuleRender(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "renderer";
	renderer = NULL;
	camera.x = camera.y = 0;
	camera.w = SCREEN_WIDTH;
	camera.h = SCREEN_HEIGHT;

	renderLayers.resize(4);
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init(pugi::xml_node& config)
{
	LOG("Creating Renderer context");
	bool ret = true;
	Uint32 flags = 0;

	this->config = config;

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
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		cameraSpeed++;
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		cameraSpeed--;

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
	{
		App->renderer->camera.y -= cameraSpeed;
		printf_s("Camera_X: %d, Camera_Y: %d\n", camera.x, camera.y);
	}
	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		App->renderer->camera.y += cameraSpeed;
		printf_s("Camera_X: %d, Camera_Y: %d\n", camera.x, camera.y);
	}
	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		App->renderer->camera.x -= cameraSpeed;
		printf_s("Camera_X: %d, Camera_Y: %d\n", camera.x, camera.y);
	}
	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		App->renderer->camera.x += cameraSpeed;
		printf_s("Camera_X: %d, Camera_Y: %d\n", camera.x, camera.y);
	}
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
UpdateStatus ModuleRender::PostUpdate()
{
	// Sorting layers
	for (int i = 0; i < renderLayers.size(); i++)
	{
		SortRenderObjects(renderLayers[i]);
	}

	//Draw
	for (int i = 0; i < 3; i++)
	{
		for each (auto renderObject in renderLayers[i])
		{
			//SDL_SetTextureAlphaMod(renderObject.texture, 100);

			renderObject.Draw(renderer);

			/*if (renderObject.section.w == 0 || renderObject.section.h == 0)
			{
				if (SDL_RenderCopyEx(renderer, renderObject.texture, nullptr, &renderObject.renderRect, renderObject.rotation, NULL, renderObject.flip) != 0)
				{
					LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
				}
			}
			else
			{
				if (SDL_RenderCopyEx(renderer, renderObject.texture, &renderObject.section, &renderObject.renderRect, renderObject.rotation, NULL, renderObject.flip) != 0)
				{
					LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
				}
			}			*/
		}
	}

	// Draw Rects
	//for (int i = 0; i < rects.size(); i++)
	//{
	//	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	//	SDL_SetRenderDrawColor(renderer, rects[i].color.r, rects[i].color.g, rects[i].color.b, rects[i].color.a);

	//	SDL_RenderFillRect(renderer, &rects[i].rect);
	//}

	// Draw Special Layer
	//for each (auto renderObject in layers[3])
	//{
	//	if (SDL_RenderCopyEx(renderer, renderObject.texture, &renderObject.section, &renderObject.renderRect, renderObject.rotation, NULL, renderObject.flip) != 0)
	//	{
	//		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
	//	}
	//}

	if (App->isDebug)
	{
		App->physics->ShapesRender();
	}
	
	SDL_RenderPresent(renderer);

	for (int i = 0; i < 4; i++)
	{
		renderLayers[i].clear();
	}

	//rects.clear();

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

void ModuleRender::AddTextureRenderQueue(SDL_Texture* texture, iPoint pos, SDL_Rect section, float scale, int layer, float orderInlayer, float rotation, SDL_RendererFlip flip, float speed)
{
	SDL_Rect destRect = { 0,0 };

	RenderObject renderObject;

	renderObject.InitAsTexture(texture, destRect, section, layer, orderInlayer, flip, rotation, scale, speed);

	if (layer == 2 || layer == 3) renderObject.speedRegardCamera = 0;	//If texture in UI layer, it moves alongside the camera. Therefor, speed = 0;

	renderObject.destRect.x = (int)(-camera.x * speed) + pos.x * App->window->scale;
	renderObject.destRect.y = (int)(-camera.y * speed) + pos.y * App->window->scale;

	if (section.h != 0 && section.w != 0)
	{
		renderObject.destRect.w = section.w;
		renderObject.destRect.h = section.h;
	}
	else
	{
		// Collect the texture size into rect.w and rect.h variables
		SDL_QueryTexture(texture, nullptr, nullptr, &renderObject.destRect.w, &renderObject.destRect.h);
	}

	renderObject.destRect.w *= scale * App->window->scale;
	renderObject.destRect.h *= scale * App->window->scale;

	renderLayers[layer].push_back(renderObject);
}

void ModuleRender::AddTextureRenderQueue(RenderObject obj)
{
	RenderObject object = obj;

	object.destRect.x = (int)(-camera.x * object.speedRegardCamera) + object.destRect.x * App->window->scale;
	object.destRect.y = (int)(-camera.y * object.speedRegardCamera) + object.destRect.y * App->window->scale;

	if (object.section.w != 0 && object.section.h != 0)
	{
		object.destRect.w = object.section.w;
		object.destRect.h = object.section.h;
	}
	else
	{
		// Collect the texture size into rect.w and rect.h variables
		SDL_QueryTexture(object.texture, nullptr, nullptr, &object.destRect.w, &object.destRect.h);
	}

	object.destRect.w *= object.scale * App->window->scale;
	object.destRect.h *= object.scale * App->window->scale;

	renderLayers[object.layer].push_back(object);
}

void ModuleRender::AddRectRenderQueue(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int layer, float orderInlayer, bool filled, float speed)
{
	RenderObject renderR;

	renderR.InitAsRect(rect, { r,g,b,a }, filled, layer, orderInlayer, speed);

	renderLayers[layer].push_back(renderR);
}

int ModuleRender::RoundToInt(int num)
{
	float divisionRes;
	divisionRes = (float)num / (float)gamePixels;

	divisionRes = lround((double)divisionRes);



	return (int)(divisionRes * gamePixels);
}

void ModuleRender::SortRenderObjects(vector<RenderObject> &obj)
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
		rec.x = (int)(camera.x + rect.x * App->window->scale);
		rec.y = (int)(camera.y + rect.y * App->window->scale);
		rec.w *= App->window->scale;
		rec.h *= App->window->scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);
	
	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool adjust, bool use_camera)
{
	bool ret = true;

	if (gamePixels != 0 && adjust)
	{
		x1 = RoundToInt(x1);
		y1 = RoundToInt(y1);
		x2 = RoundToInt(x2);
		y2 = RoundToInt(y2);
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, -camera.x + x1 * App->window->scale, -camera.y + y1 * App->window->scale, -camera.x + x2 * App->window->scale, -camera.y + y2 * App->window->scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * App->window->scale, y1 * App->window->scale, x2 * App->window->scale, y2 * App->window->scale);

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
		points[i].x = (int)(-camera.x + x * App->window->scale + radius * cos(i * factor) * App->window->scale);
		points[i].y = (int)(-camera.y + y * App->window->scale + radius * sin(i * factor) * App->window->scale);
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
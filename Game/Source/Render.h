#ifndef __RENDER_H__
#define __RENDER_H__

#include "Module.h"

#include "Point.h"
#include <vector>

#include "SDL/include/SDL.h"

using namespace std;

struct RenderObject
{
	SDL_Texture* texture = nullptr;
	SDL_Rect section;
	SDL_Rect renderRect = { 0,0,0,0 };
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	float rotation = 0.0f;
	int layer = 0;
	float orderInLayer = 0.0f;
	float speed = 1.0f;
	float scale = 1.0f;

	bool rotationEnabled = true;
	bool followPhysBody = true;
};

struct RenderRect
{
	SDL_Rect rect;
	SDL_Color color;
};

class Render : public Module
{
public:

	Render();

	// Destructor
	virtual ~Render();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();

	// Drawing
	void AddTextureRenderQueue(RenderObject object);
	void AddTextureRenderQueue(SDL_Texture* texture, iPoint pos, SDL_Rect* section = nullptr, float scale = 1.0f, int layer = 0, float orderInlayer = 0.0f, float rotation = 0, SDL_RendererFlip flip = SDL_FLIP_NONE, float speed = 1.0f);// Speed = 1.0f = Fullscreen camera
	void AddRectRenderQueue(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true);
	void SortRenderObjects(vector<RenderObject>& obj);


	// Drawing OBSOLETE
	bool DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, int pivotX = INT_MAX, int pivotY = INT_MAX) const;
	bool DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool useCamera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;

	// Set background color
	void SetBackgroundColor(SDL_Color color);

public:

	SDL_Renderer* renderer;
	SDL_Rect camera;
	SDL_Rect viewport;
	SDL_Color background;

private:
	float defaultSpeed = 1.0f;

	vector<vector<RenderObject>> layers;
	vector<RenderRect> rects;
};

#endif // __RENDER_H__
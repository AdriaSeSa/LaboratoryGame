#pragma once
#include "Application.h"

enum RenderType
{
	RENDER_TEXTURE,
	RENDER_RECT,
	RENDER_LINE,
	RENDER_CIRCLE
};

class RenderObject
{
protected:
	RenderObject(SDL_Rect destRect, RenderType type, int layer, float orderInLayer, float scale, float speedRegardCamera)
	{
		this->destRect = destRect;
		this->type = type;
		this->layer = layer;
		this->orderInLayer = orderInLayer;
		this->scale = scale;
		this->speedRegardCamera = speedRegardCamera;
	};

public:

	SDL_Rect destRect;
	RenderType type;
	int layer = 0;
	float orderInLayer = 0.0f;
	float scale = 1.0f;
	float speedRegardCamera = 1.0f;
};

class RenderTexture :public RenderObject
{
public:

	RenderTexture(SDL_Texture* texture, SDL_Rect destRect, SDL_Rect section, int layer = 0, float orderInLayer = 0.0f,
		SDL_RendererFlip flip = SDL_FLIP_NONE, float rotation = 0.0f, float scale = 1.0f, float speedRegardCamera = 1.0f)
		:RenderObject(destRect, RenderType::RENDER_TEXTURE, layer, orderInLayer, scale, speedRegardCamera)
	{
		this->texture = texture;
		this->section = section;
		this->flip = flip;
		this->rotation = rotation;
	}

	SDL_Texture* texture = nullptr;
	SDL_Rect section = { 0,0,0,0 };
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	float rotation = 0.0f;
};

class RenderRect :public RenderObject
{
public:
	RenderRect(SDL_Rect destRect, SDL_Color color = { 0,0,0,255 }, int layer = 0, float orderInLayer = 0.0f,
		float scale = 1.0f, float speedRegardCamera = 1.0f) :RenderObject(destRect, RenderType::RENDER_RECT, layer, orderInLayer, scale, speedRegardCamera)
	{
		this->color = color;
	}

	SDL_Color color;
};
#pragma once
#include "GUI.h"
#include "ModuleRender.h"

enum class ButtonState
{
	IDLE,
	FOCUS,
	PRESSED
};

class GUIButton : public GUI
{
private:
	RenderObject* renderObject = nullptr;

	SDL_Rect renderSections[3];

	ButtonState buttonState = ButtonState::IDLE;

public:
	GUIButton(Application* app, iPoint pos, int width, int height, std::string path);

	~GUIButton();

	void Update() override;

	void PostUpdate() override;

	bool isPressed = false;
	bool doAction = false;
};


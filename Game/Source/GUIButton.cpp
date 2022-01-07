#include "GUIButton.h"
#include "Application.h"

GUIButton::GUIButton(Application* app, iPoint pos, int width, int height, std::string path) : GUI(app)
{
	InitAsBox(pos.x, pos.y, width, height);

	renderObject = new RenderObject();
	renderObject->texture = _app->textures->Load(path);
	renderObject->destRect = { pos.x, pos.y, width, height };
	renderObject->layer = 3;
	renderObject->orderInLayer = 10;
	renderObject->scale = 1;

	// The Button texture must be three images of the different states of the button, organized from top to bottom in order of:
	// IDLE, FOCUS, PRESSED
	for (int i = 0; i < 3; i++)
	{
		renderSections[i].x = 0;
		renderSections[i].y = i * height;
		renderSections[i].w = width;
		renderSections[i].h = height;
	}
}

GUIButton::GUIButton(Application* app, iPoint pos, int width, int height) : GUI(app)
{
	InitAsBox(pos.x, pos.y, width, height);
}

GUIButton::~GUIButton()
{
	RELEASE(renderObject);
}

void GUIButton::Update()
{
	if (CheckOnMouse() && (_app->input->GetMouseButton(1) == KEY_DOWN))
	{
		// Sound Effect
		buttonState = ButtonState::PRESS_DOWN;
		isPressed = true;
	}
	else if (isPressed && _app->input->GetMouseButton(1) == KEY_REPEAT)
	{
		// Sound Effect
		buttonState = ButtonState::PRESSED;
	}
	else if (CheckOnMouse())
	{
		// Sound Effect
		buttonState = ButtonState::FOCUS;
	}
		
	if (_app->input->GetMouseButton(1) == KEY_UP)
	{
		// We do the action
		if (isPressed && CheckOnMouse())
		{
			//printf("Mouse Action\n");
			doAction = true;
		}
		isPressed = false;
		buttonState = ButtonState::IDLE;
	}
	else if (!CheckOnMouse())
	{
		if (!navigation || !isPressed)
		{
			buttonState = ButtonState::IDLE;
			isPressed = false;
		}
	}

	switch (buttonState)
	{
		case ButtonState::IDLE:
			defaultColor = { 255, 255, 255, 255};
		break;

		case ButtonState::FOCUS:
			defaultColor = { 200, 200, 200, 255};
			break;

		case ButtonState::PRESS_DOWN:
		case ButtonState::PRESSED:
			defaultColor = { 100, 0, 0, 255};
			break;
	}
}

void GUIButton::PostUpdate()
{
	if (renderObject != nullptr && renderObject->texture != nullptr)
	{
		int newBtnState = CLAMP((int)buttonState, 0, 2);

		_app->renderer->AddTextureRenderQueue(renderObject->texture, { position.x, position.y }, renderSections[newBtnState], renderObject->scale,
											  renderObject->layer, renderObject->orderInLayer);
	}
	else
	{
		_app->renderer->AddRectRenderQueue(SDL_Rect{ position.x,position.y,boxShape.w,boxShape.h }, defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a, 3, 1);
	}
}

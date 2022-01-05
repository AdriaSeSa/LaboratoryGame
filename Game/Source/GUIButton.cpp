#include "GUIButton.h"
#include "Application.h"

GUIButton::GUIButton(Application* app, iPoint pos, int width, int height, std::string path) : GUI(app)
{
	InitAsBox(pos.x, pos.y, width, height);

	renderObject = new RenderObject();
	renderObject->texture = _app->textures->Load(path);
	renderObject->destRect = { pos.x, pos.y, width, height };
	renderObject->layer = 3;
	renderObject->orderInLayer = 1;
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

GUIButton::~GUIButton()
{
	RELEASE(renderObject);
}

void GUIButton::Update()
{
	if (CheckOnMouse() && (_app->input->GetMouseButton(1) == KEY_DOWN))
	{
		// Sound Effect
		buttonState = ButtonState::PRESSED;
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
	else
	{
		buttonState = ButtonState::IDLE;
	}
	if (_app->input->GetMouseButton(1) == KEY_UP)
	{
		// We do the action
		if (isPressed && CheckOnMouse())
		{
			printf("Mouse Action\n");
			doAction = true;
		}
		isPressed = false;
	}
}

void GUIButton::PostUpdate()
{
	if (renderObject->texture != nullptr)
	{
		_app->renderer->AddTextureRenderQueue(renderObject->texture, { renderObject->destRect.x, renderObject->destRect.y }, renderSections[(int)buttonState], renderObject->scale,
											  renderObject->layer, renderObject->orderInLayer);
	}
}

#include "GUISlider.h"
#include "GUIButton.h"
#include "Application.h"

GUISlider::GUISlider(Application* app, iPoint pos, int width, int height, std::string path) : GUI(app)
{
	// Init Slider
	InitAsBox(pos.x, pos.y, width, height);

	renderObject = new RenderObject();
	renderObject->texture = _app->textures->Load(path);
	renderObject->destRect = { pos.x, pos.y, width, height };
	renderObject->layer = 3;
	renderObject->orderInLayer = 10;
	renderObject->scale = 1;

	// The Slider texture must be two images of the different states of the slider, organized from top to bottom in order of:
	// EMPTY, FILLED
	for (int i = 0; i < 2; i++)
	{
		renderSections[i].x = 0;
		renderSections[i].y = i * height;
		renderSections[i].w = width;
		renderSections[i].h = height;
	}
}

GUISlider::GUISlider(Application* app, iPoint pos, int width, int height) : GUI(app)
{
	// Init Slider
	InitAsBox(pos.x, pos.y, width, height);

	min_value = position.x;
	max_value = position.x + boxShape.w - btn->boxShape.w;

	defaultColor = { 11, 152, 217, 255};
}

GUISlider::~GUISlider()
{
	RELEASE(btn);
	RELEASE(renderObject);
}

void GUISlider::CreateDefaultBtn()
{
	// Init Btn
	if (btn == nullptr)
	{
		// Init button
		btn = new GUIButton(_app, position, boxShape.h, boxShape.h);
		btn->navigation = true;

		// Init min max value
		min_value = position.x;
		max_value = position.x + boxShape.w - btn->boxShape.w;
	}
	else
	{
		printf("Button has created!!!");
	}
}

void GUISlider::CreateGUIBtn(GUIButton* btn)
{
	if (this->btn == nullptr)
	{
		this->btn = btn;
		btn->navigation = true;

		// Init min max value
		min_value = position.x;
		max_value = position.x + boxShape.w - btn->boxShape.w;

		btn->position.y += (boxShape.h - btn->boxShape.h) / 2;
	}
	else
	{
		printf("Button has created!!!");
	}
}

void GUISlider::Update()
{
	if (btn != nullptr)
	{
		btn->Update();
		iPoint mousePos = { _app->input->GetMouseX(), _app->input->GetMouseY() };
		int btnNewPos_x = 0;
		
		float screenOffset = _app->FullScreenDesktop ? (float)_app->renderer->displayMode.h / 640.0f : 1;
		float widthOffset = _app->FullScreenDesktop ? (_app->renderer->displayMode.w - (640.0f * screenOffset)) / 2 : 0;

		switch (btn->buttonState)
		{
		case ButtonState::PRESS_DOWN:

			//Get Offset of mouse.x and btnpos.x
			btnMouse_offset_x = (mousePos.x / (_app->window->scale * screenOffset)) - btn->position.x;
			break;

		case ButtonState::PRESSED:

			// Calculate new btnpos
			btnNewPos_x = (mousePos.x / (_app->window->scale * screenOffset)) - btnMouse_offset_x;

			printf("B: %d\n", btnNewPos_x);

			// Change button position after Clamping
			btn->position.x = CLAMP(btnNewPos_x, min_value, max_value);

			// Update value;
			value = btn->position.x - min_value;

			break;

		default:
			if (CheckOnMouse())
			{
				if (_app->input->GetMouseButton(1) == KEY_DOWN)
				{
					btnMouse_offset_x = (btn->boxShape.w / 2);

					btnNewPos_x = ((mousePos.x - widthOffset) / (_app->window->scale * screenOffset)) - btnMouse_offset_x;

					btnMouse_offset_x = (mousePos.x / (_app->window->scale * screenOffset)) - btnNewPos_x;

					// Change button position after Clamping
					btn->position.x = CLAMP(btnNewPos_x, min_value, max_value);

					// Update value;
					value = btn->position.x - min_value;

					btn->buttonState = ButtonState::PRESSED;
					btn->isPressed = true;
				}
			}
			break;
		}
	}
}

void GUISlider::PostUpdate()
{
	if (renderObject != nullptr && renderObject->texture != nullptr)
	{
		_app->renderer->AddTextureRenderQueue(renderObject->texture, { renderObject->destRect.x, renderObject->destRect.y }, renderSections[0], renderObject->scale,
			renderObject->layer, renderObject->orderInLayer, 0, SDL_FLIP_NONE, 0);

		renderSections[1].w = renderSections[0].w * GetValue();

		if (renderSections[1].w > 0)
		{
			_app->renderer->AddTextureRenderQueue(renderObject->texture, { renderObject->destRect.x, renderObject->destRect.y }, renderSections[1], renderObject->scale,
				renderObject->layer, renderObject->orderInLayer + 1, 0, SDL_FLIP_NONE, 0);
		}
	}
	else
	{
		_app->renderer->AddRectRenderQueue(SDL_Rect{ position.x,position.y,boxShape.w,boxShape.h }, defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a, 3,0);
	}

	if (btn != nullptr)
	{
		btn->PostUpdate();
	}
}

void GUISlider::SetValue(float val)
{
	val = val > 1 ? 1 : val < 0 ? 0 : val;

	float total_value = max_value - min_value;

	// Change value
	this->value = total_value * val;

	// Change btn position
	btn->position.x = CLAMP(min_value+ value, min_value, max_value);
}

float GUISlider::GetValue()
{
	float total_value = max_value - min_value;

	return (value / total_value);
}

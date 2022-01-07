#include "GUICheckbox.h"
#include "Application.h"

GUICheckbox::GUICheckbox(Application* app, iPoint pos, int width, int height, std::string path) : GUI(app)
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

GUICheckbox::~GUICheckbox()
{
	RELEASE(renderObject);
}

void GUICheckbox::Update()
{
	if (CheckOnMouse() && _app->input->GetMouseButton(1) == KEY_UP)
	{
		ChangeState(!isActive);
	}
	else if (CheckOnMouse() && checkboxState != CheckboxState::ON) checkboxState = CheckboxState::FOCUS;
	else if (checkboxState != CheckboxState::ON) checkboxState = CheckboxState::OFF;
}

void GUICheckbox::PostUpdate()
{
	printf("%d", isActive);
	if (renderObject != nullptr && renderObject->texture != nullptr)
	{
		_app->renderer->AddTextureRenderQueue(renderObject->texture, { position.x, position.y }, renderSections[(int)checkboxState], renderObject->scale,
			renderObject->layer, renderObject->orderInLayer);
	}
}

void GUICheckbox::ChangeState(bool isOn)
{
	isActive = isOn;
	checkboxState = isActive ? CheckboxState::ON : CheckboxState::OFF;
}

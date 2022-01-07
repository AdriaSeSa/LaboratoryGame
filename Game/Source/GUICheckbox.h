#pragma once
#include "GUI.h"
#include "ModuleRender.h"

enum class CheckboxState
{
	OFF,
	FOCUS,
	ON
};

class GUICheckbox : public GUI
{
public:
	GUICheckbox(Application* app, iPoint pos, int width, int height, std::string path);

	~GUICheckbox();

	void Update() override;

	void PostUpdate() override;

	void ChangeState(bool isOn);

private:
	RenderObject* renderObject = nullptr;

	SDL_Rect renderSections[3];

	CheckboxState checkboxState = CheckboxState::OFF;

public:
	bool isActive = false;
};


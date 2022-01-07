#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__
#include "GUI.h"
#include "string"

class GUIButton;
class RenderObject;

class GUISlider :public GUI
{
private:
	RenderObject* renderObject = nullptr;

	SDL_Rect renderSections[2];

	GUIButton* btn = nullptr;

	float value = 0;

	float max_value = 100;

	float min_value = 0;

	int btnMouse_offset_x = 0;

public:
	GUISlider(Application* app, iPoint pos, int width, int height, std::string path);

	GUISlider(Application* app, iPoint pos, int width, int height);

	~GUISlider();

	void CreateDefaultBtn();

	void CreateGUIBtn(GUIButton* btn);

	void Update() override;

	void PostUpdate() override;

	float GetValue();
};

#endif // !__GUISLIDER_H__
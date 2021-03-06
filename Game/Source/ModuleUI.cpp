#include "Application.h"

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled) 
{
	name = "ui";
}

bool ModuleUI::Init(pugi::xml_node& config)
{
	this->config = config;

	return true;
}

bool ModuleUI::Start()
{
	texture = App->textures->Load("Assets/fonts/font.png");

	for (int i = 0; i < 10; i++)
	{
		numSection[i].x = 25 * i;
		numSection[i].y = 0;
		numSection[i].w = 25;
		numSection[i].h = 32;
	}

    return true;
}

UpdateStatus ModuleUI::Update()
{
	for (int i = 0; i < MAX_UI_ITEMS; i++)
	{
		if (uiArray[i] == nullptr) continue;

		if (uiArray[i]->isDynamic)
		{
			if (uiArray[i]->lifeFrames > 0)
			{
				uiArray[i]->lifeFrames--;
				uiArray[i]->x += uiArray[i]->speed.x;
				uiArray[i]->y += uiArray[i]->speed.y;
			}
			else
			{
				DestroyUI(i);
			}
		}
	}

	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleUI::PostUpdate()
{
	//Draw all current UIs
	for (int i = 0; i < MAX_UI_ITEMS; i++)
	{
		if (uiArray[i] != nullptr)
		{
			for (int j = 0; j < uiArray[i]->totalDigits; j++)
			{
				iPoint tempPos = iPoint(uiArray[i]->x, uiArray[i]->y);
				tempPos.x += (int)(25 * uiArray[i]->digitScale * j); // Spacing between digits
				int speed = uiArray[i]->isDynamic ? 1 : 0;
				App->renderer->AddTextureRenderQueue(texture, tempPos, numSection[uiArray[i]->digitVec.at(j)], uiArray[i]->digitScale, uiArray[i]->layer, uiArray[i]->orderInLayer, 0, SDL_FLIP_NONE, speed);
			}
		}
	}
	return UpdateStatus::UPDATE_CONTINUE;
}

/// <summary>
/// Get number and position of the UI
/// Make the number into digits
/// Store it into an UI item in the ui array
/// It prints on screen on the PostUpdate function
/// </summary>
/// <param name="num"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
uint ModuleUI::CreateUI(int num, int x, int y, float scale, int layer, float orderInLayer, bool isDynamic, int lifeFrames, iPoint speed)
{
	//  Get position of the UI
	itemUI* item = new itemUI();
	item->x = x;
	item->y = y;
	item->digitScale = scale;
	item->layer = layer;
	item->orderInLayer = orderInLayer;
	item->isDynamic = isDynamic;
	item->lifeFrames = lifeFrames;
	item->speed = speed;

	//Make the number into an array of digits
	//	Declare Variables
	std::stack<int> digits;

	//Getting digits from number in order Logic
	if (num != 0) {
		while (num > 0)
		{
			int digit = num % 10;
			num /= 10;
			digits.push(digit);
		}

		item->totalDigits = digits.size();

		while (!digits.empty())
		{
			int digit = digits.top();
			item->digitVec.push_back(digit);
			digits.pop();
		}
	}
	else
	{
		item->digitVec.push_back(0);
		item->totalDigits = 1;
	}


	for (int i = 0; i < MAX_UI_ITEMS; i++)
	{
		if (uiArray[i] == nullptr)
		{
			uiArray[i] = item;
			return (uint)i;
		}
	}
}

void ModuleUI::DestroyUI(uint index)
{
	//if (uiArray[index] == nullptr)return;
	delete uiArray[index];
	uiArray[index] = nullptr;
}

bool ModuleUI::CleanUp()
{
	for (int i = 0; i < MAX_UI_ITEMS; i++)
	{
		if (uiArray[i] != nullptr)
		{
			delete uiArray[i];
			uiArray[i] = nullptr;
		}
	}

	return true;
}

void itemUI::ChangeUI(int num)
{
	//Make the number into an array of digits
	//	Declare Variables
	std::stack<int> digits;

	this->digitVec.clear();

	//Getting digits from number in order Logic
	if (num != 0) {
		while (num > 0)
		{
			int digit = num % 10;
			num /= 10;
			digits.push(digit);
		}

		this->totalDigits = digits.size();

		while (!digits.empty())
		{
			int digit = digits.top();
			this->digitVec.push_back(digit);
			digits.pop();
		}
	}
	else
	{
		this->digitVec.push_back(0);
		this->totalDigits = 1;
	}
}

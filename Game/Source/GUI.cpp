#include "GUI.h"
#include "Application.h"


GUI::GUI(Application* app)
{
    _app = app;
}

void GUI::InitAsBox(int x, int y, int w, int h)
{
    currentShape = UIShape::BOX;
    boxShape.h = h;
    boxShape.w = w;
    position.x = x;
    position.y = y;
}

void GUI::InitAsCircle(int x, int y, int radius)
{
    currentShape = UIShape::CIRCLE;
    circleShape.radius = radius;
    position.x = x;
    position.y = y;
}

void GUI::Update()
{
}

bool GUI::CheckOnMouse()
{
    iPoint mousePos = { _app->input->GetMouseX(), _app->input->GetMouseY()};
    printf("x: %d\ty:%d\n", mousePos.x, mousePos.y);
    if (currentShape == UIShape::CIRCLE)
    {
        if (mousePos.DistanceTo(position * _app->window->scale) < circleShape.radius * _app->window->scale) return true;
    }
    else
    {
        if (mousePos.x < position.x  * _app->window->scale|| mousePos.x > (position.x + boxShape.w) * _app->window->scale
            || mousePos.y < position.y * _app->window->scale || mousePos.y > (position.y + boxShape.h) * _app->window->scale) return false;
        else return true;
    }
    return false;
}


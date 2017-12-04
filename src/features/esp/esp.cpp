#include "esp.hpp"

#include "../../aim-flex.hpp"

void ESP::Init()
{

}

void ESP::Invoke()
{
	Vector screen;

	if (WorldToScreen(Vector(0, 0, 0), screen))
	{
		g_MatSystemSurface->SetDrawColor(Color(255, 0, 0));
		g_MatSystemSurface->DrawFilledRect((int)screen.x, (int)screen.y, 5, 5);
	}
}

void ESP::Destroy()
{

}

extern ESP esp;

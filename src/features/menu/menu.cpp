#include "menu.hpp"

#include "../../aim-flex.hpp"

#include "../input/input.hpp"

void Menu::Init()
{
	matsystemsurface->SetFontGlyphSet(font = matsystemsurface->CreateFont(), "Arial", 18, 500, 0, 0, FONTFLAG_OUTLINE);
	matsystemsurface->SetFontGlyphSet(font_small = matsystemsurface->CreateFont(), "Arial", 16, 500, 0, 0, FONTFLAG_OUTLINE);

	lists.AddList(settings.GetSettings());

	input.OnKey(VK_INSERT, [this]()
	{
		menu.open = !menu.open;
	});
}

void Menu::Invoke()
{
	if (menu.open)
		lists.Draw();
}

int BaseTab::GetFont()
{
	return menu.font;
}

int BaseTab::GetSmallFont()
{
	return menu.font_small;
}

bool BaseTab::IsMenuOpen()
{
	return menu.open;
}

void Menu::Destroy()
{

}

Menu menu;

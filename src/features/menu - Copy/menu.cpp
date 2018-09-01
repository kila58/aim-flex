#include "menu.hpp"

#include "../../aim-flex.hpp"

#include "../input/input.hpp"

void Menu::Init()
{
	matsystemsurface->SetFontGlyphSet(font = matsystemsurface->CreateFont(), "Arial", 18, 500, 0, 0, FONTFLAG_OUTLINE);
	matsystemsurface->SetFontGlyphSet(font_small = matsystemsurface->CreateFont(), "Arial", 16, 500, 0, 0, FONTFLAG_OUTLINE);

	lists.emplace_back(TabList())
		.Construct(settings.GetSettings());
}

void BaseTab::Draw()
{
	auto[x, y] = GetPosition();
	auto title = GetTitle();

	if (highlighted)
		matsystemsurface->SetDrawColor(Color(22, 22, 22));
	else
		matsystemsurface->SetDrawColor(Color(29, 29, 29));

	matsystemsurface->DrawFilledRect(x, y, basetab::tab_w, basetab::tab_h);

	matsystemsurface->SetTextColor(Color(186, 186, 186));
	matsystemsurface->SetFont(menu.font);

	int tw, th;
	matsystemsurface->GetTextSize(menu.font, title, tw, th);

	matsystemsurface->SetTextPos(x + basetab::tab_title_left_pad, y + (basetab::tab_h / 2) - (th / 2));
	matsystemsurface->DrawPrintText(title);

	DrawArrow();
}

void BaseTab::DrawArrow()
{
	matsystemsurface->SetTextColor(Color(186, 186, 186));
	matsystemsurface->SetFont(menu.font);

	std::string arrow = highlighted ? ">" : "<";

	int tw, th;
	matsystemsurface->GetTextSize(menu.font, arrow, tw, th);

	matsystemsurface->SetTextPos(x + basetab::tab_w - basetab::tab_title_left_pad - tw, y + (basetab::tab_h / 2) - (th / 2));
	matsystemsurface->DrawPrintText(arrow);
}

TabList& TabList::AddTabList(TabList& new_list)
{
	return menu.lists.emplace_back(new_list);
}

void TabList::ClearTabListSublevels()
{
	for (auto it = menu.lists.begin(); it != menu.lists.end(); )
	{
		auto index = std::distance(menu.lists.begin(), it);

		if (index != 0)
		{
			it = menu.lists.erase(it);

			continue;
		}

		++it;
	}

	sublevel_counter = 1;

	//menu.mark_for_clear = true;
}

void Menu::Invoke()
{
	for (auto& list : lists)
	{
		list.Draw();
	}

	//if (mark_for_clear)
	if (false)
	{

		for (auto it = menu.lists.begin(); it != menu.lists.end(); ) 
		{
			auto index = std::distance(menu.lists.begin(), it);

			if (index != 0)
			{
				it = menu.lists.erase(it);

				continue;
			}
			
			++it;
		}

		/*
		for (auto it = menu.lists.begin(); it != menu.lists.end(); ++it)
		{
			if (first)
				first = false;
			else
			{
				menu.lists.erase(it);
			}
		}
		*/

		menu.mark_for_clear = false;
	}
}

void Menu::Destroy()
{

}

Menu menu;

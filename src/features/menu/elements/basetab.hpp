#pragma once

#include "../../../aim-flex.hpp"

namespace basetab
{
	const int tab_w = (16 * 16) /*256*/;
	const int tab_h = 36;
	const int tab_title_left_pad = 7;
	const int tab_divider_y = 2;
};

enum tab_type
{
	type_basetab = 1,
	type_toggletab,
	type_dropdown,
	type_dropdown_option,
	type_slider
};

class BaseTab
{
protected:
	std::string name = "Tab";
	int x = 0, y = 0;
	bool highlighted = false;
	bool has_sublist = false;
	tab_type type = type_basetab;
public:
	BaseTab(std::string& name, bool has_sublist) : name(name), has_sublist(has_sublist) {}

	inline std::pair<int, int> GetPosition() const
	{
		return std::make_pair(x, y);
	}
	inline void SetPosition(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	inline void SetX(int x)
	{
		this->x = x;
	}
	inline void SetY(int y)
	{
		this->y = y;
	}
	inline void SetHighlighted(bool current)
	{
		highlighted = current;
	}
	inline bool GetHighlighted()
	{
		return highlighted;
	}
	inline std::string& GetTitle()
	{
		return name;
	}
	inline bool HasSublist()
	{
		return has_sublist;
	}
	// definition of BaseTab::GetFont is in menu.cpp
	int GetFont();
	int GetSmallFont();
	bool IsMenuOpen();
	virtual void Draw();
	virtual void OnPressedRight() {};
	// overrides backwards functionality
	virtual bool OnPressedLeft() { return false;  };
	void DrawArrow();
};

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
	matsystemsurface->SetFont(GetFont());

	int tw, th;
	matsystemsurface->GetTextSize(GetFont(), title, tw, th);

	matsystemsurface->SetTextPos(x + basetab::tab_title_left_pad, y + (basetab::tab_h / 2) - (th / 2));
	matsystemsurface->DrawPrintText(title);

	if (has_sublist)
		DrawArrow();
}

void BaseTab::DrawArrow()
{
	matsystemsurface->SetTextColor(Color(186, 186, 186));
	matsystemsurface->SetFont(GetFont());

	std::string arrow = highlighted ? ">" : "<";

	int tw, th;
	matsystemsurface->GetTextSize(GetFont(), arrow, tw, th);

	matsystemsurface->SetTextPos(x + basetab::tab_w - basetab::tab_title_left_pad - tw, y + (basetab::tab_h / 2) - (th / 2));
	matsystemsurface->DrawPrintText(arrow);
}

#pragma once

class DropdownTab : public BaseTab
{
private:
	tab_type type = type_dropdown;
	json& ref_to_setting;
public:
	DropdownTab(std::string name, bool has_sublist, json& ref_to_setting) : BaseTab(name, has_sublist), ref_to_setting(ref_to_setting) {}

	void Draw();
};

void DropdownTab::Draw()
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

	matsystemsurface->SetTextColor(Color(136, 136, 136));
	matsystemsurface->SetFont(GetSmallFont());

	if (ref_to_setting[3] != json::value_t::null)
	{
		std::string status = "(" + ref_to_setting[3].get<std::string>() + ")";

		int tw2, th2;
		matsystemsurface->GetTextSize(GetSmallFont(), status, tw2, th2);

		matsystemsurface->SetTextPos(x + basetab::tab_title_left_pad + tw + basetab::tab_title_left_pad, y + (basetab::tab_h / 2) - (th2 / 2));
		matsystemsurface->DrawPrintText(status);
	}

	DrawArrow();
}

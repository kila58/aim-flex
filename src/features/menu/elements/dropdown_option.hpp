#pragma once

class DropdownOptionTab : public BaseTab
{
private:
	tab_type type = type_dropdown_option;
	json& ref_to_super_setting;
public:
	DropdownOptionTab(std::string name, bool has_sublist, json& ref_to_super_setting) : BaseTab(name, has_sublist), ref_to_super_setting(ref_to_super_setting) {}

	bool FindSettingFromTitle();
	void Draw();
	void OnPressedRight();
};

bool DropdownOptionTab::FindSettingFromTitle()
{
	auto title = GetTitle();

	for (auto& i : ref_to_super_setting)
	{
		if (i.is_string())
		{
			if (i == title)
				return true;
		}
	}

	return false;
}

void DropdownOptionTab::Draw()
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

	if (FindSettingFromTitle())
	{
		std::wstring checkmark = L"\u2713";

		matsystemsurface->GetTextSize(GetFont(), checkmark.c_str(), tw, th);

		tw -= 5;

		matsystemsurface->SetTextPos(x + basetab::tab_w - basetab::tab_title_left_pad - tw, y + (basetab::tab_h / 2) - (th / 2));
		matsystemsurface->DrawPrintText(checkmark.c_str());
	}
}

void DropdownOptionTab::OnPressedRight()
{
	auto& set_option = ref_to_super_setting[3];

	set_option = GetTitle();
}

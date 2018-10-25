#pragma once

class ToggleTab : public BaseTab
{
private:
	tab_type type = type_toggletab;
	json& ref_to_setting;
public:
	ToggleTab(const std::string& name, bool has_sublist, json& ref_to_setting) : BaseTab(name, has_sublist), ref_to_setting(ref_to_setting) {}

	void Draw();
	void OnPressedRight();
};

void ToggleTab::Draw()
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

	auto& enabled = ref_to_setting[2];

	if (enabled)
		matsystemsurface->SetTextColor(Color(186, 186, 186));
	else
		matsystemsurface->SetTextColor(Color(146, 146, 146, 215));

	std::string status = enabled ? "on" : "off";

	matsystemsurface->GetTextSize(GetFont(), status, tw, th);

	matsystemsurface->SetTextPos(x + basetab::tab_w - basetab::tab_title_left_pad - tw, y + (basetab::tab_h / 2) - (th / 2));
	matsystemsurface->DrawPrintText(status);
}

void ToggleTab::OnPressedRight()
{
	auto& enabled = ref_to_setting[2];

	enabled = !enabled;
}

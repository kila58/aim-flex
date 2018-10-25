#pragma once

class SliderTab : public BaseTab
{
private:
	tab_type type = type_slider;
	json& ref_to_setting;
public:
	SliderTab(const std::string& name, bool has_sublist, json& ref_to_setting) : BaseTab(name, has_sublist), ref_to_setting(ref_to_setting) {}

	void Draw();
	void OnPressedRight();
	bool OnPressedLeft();
};

void SliderTab::Draw()
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

	int value = ref_to_setting[2];
	int min = ref_to_setting[3];
	int max = ref_to_setting[4];

	std::string symbol = ref_to_setting[5];
	int tw2, th2;

	/*
	debug << symbol << "\n";

	if (symbol == "a")
	{
		std::string to_convert = std::to_string(value);
		int len = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, to_convert.c_str(), to_convert.size(), NULL, 0);

		if (!len)
			return;

		std::wstring str(len, 0);
		MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, to_convert.c_str(), to_convert.size(), str.data(), str.size());

		std::wstring status = L"(" + str + L"\u00B0" + L")";

		matsystemsurface->GetTextSize(GetSmallFont(), status.c_str(), tw2, th2);

		matsystemsurface->SetTextPos(x + basetab::tab_title_left_pad + tw + basetab::tab_title_left_pad, y + (basetab::tab_h / 2) - (th2 / 2));
		matsystemsurface->DrawPrintText(status.c_str());
	}
	else
	*/
	{
		std::string status = "(" + std::to_string(value) + symbol + ")";

		matsystemsurface->GetTextSize(GetSmallFont(), status, tw2, th2);

		matsystemsurface->SetTextPos(x + basetab::tab_title_left_pad + tw + basetab::tab_title_left_pad, y + (basetab::tab_h / 2) - (th2 / 2));
		matsystemsurface->DrawPrintText(status);
	}

	matsystemsurface->SetDrawColor(Color(0, 0, 0));

	int start_slider_x = x + basetab::tab_title_left_pad + tw + tw2 + (basetab::tab_title_left_pad * 2);
	int start_slider_y = y + (basetab::tab_h / 2);

	int end_slider_x = x + basetab::tab_w - basetab::tab_title_left_pad;
	int end_slider_y = start_slider_y;

	// alignment (will still be off by like 1px, but whatever...)
	start_slider_y = start_slider_y - 1;
	end_slider_y = end_slider_y - 1;

	// DrawFilledRect? meh...
	matsystemsurface->DrawLine(start_slider_x, start_slider_y, end_slider_x, end_slider_y);
	matsystemsurface->DrawLine(start_slider_x, start_slider_y + 1, end_slider_x, end_slider_y + 1);
	matsystemsurface->DrawLine(start_slider_x, start_slider_y + 2, end_slider_x, end_slider_y + 2);

	int bar_width = end_slider_x - start_slider_x;

	if (min == 1)
		min = 0;

	int width = (int)((std::clamp((float)value / (float)max, (float)min, (float)max)) * 
		(bar_width - 2)); // clip front and end 1px for outline

	matsystemsurface->SetDrawColor(Color(186, 186, 186));
	matsystemsurface->DrawLine(start_slider_x + 1, start_slider_y + 1, start_slider_x + width + 1, end_slider_y + 1);
}

void SliderTab::OnPressedRight()
{
	ref_to_setting[2] = std::clamp<int>(ref_to_setting[2] + 1, ref_to_setting[3], ref_to_setting[4]);
}

bool SliderTab::OnPressedLeft()
{
	ref_to_setting[2] = std::clamp<int>(ref_to_setting[2] - 1, ref_to_setting[3], ref_to_setting[4]);

	return true;
}

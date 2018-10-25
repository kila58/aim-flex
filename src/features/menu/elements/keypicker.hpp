#pragma once

class KeyPickerTab : public BaseTab
{
private:
	tab_type type = type_toggletab;
	json& ref_to_setting;
	bool picking = false;
	bool first = false;
	int key = 0x0;
public:
	KeyPickerTab(const std::string& name, bool has_sublist, json& ref_to_setting) : BaseTab(name, has_sublist), ref_to_setting(ref_to_setting) {}

	void Draw();
	void OnPressedRight();
};

std::string VirtualKeyCodeToString(UCHAR key)
{
	UINT code = MapVirtualKeyA(key, MAPVK_VK_TO_VSC);
	CHAR name[128];

	switch (key)
	{
	case VK_LBUTTON:
		strcpy_s(name, "left mouse");
		break;
	case VK_RBUTTON:
		strcpy_s(name, "right mouse");
		break;
	case VK_XBUTTON1:
		strcpy_s(name, "mouse 4");
		break;
	case VK_XBUTTON2:
		strcpy_s(name, "mouse 5");
		break;
	case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
	case VK_PRIOR: case VK_NEXT:
	case VK_END: case VK_HOME:
	case VK_INSERT: case VK_DELETE:
	case VK_DIVIDE:
	case VK_NUMLOCK:
		code |= KF_EXTENDED;
	default:
		GetKeyNameTextA(code << 16, name, 128);
	}

	return name;
}

void KeyPickerTab::Draw()
{
	if (!first)
	{
		first = true;

		input.OnAnyKey([this](int key)
		{
			if (picking)
			{
				if (!(key == VK_RIGHT || key == VK_LEFT || key == VK_DOWN || key == VK_UP || key == VK_INSERT)) // lmao
					this->key = key;
			}
		});
	}

	auto[x, y] = GetPosition();
	auto title = GetTitle();

	if (highlighted)
		matsystemsurface->SetDrawColor(Color(22, 22, 22));
	else
	{
		matsystemsurface->SetDrawColor(Color(29, 29, 29));

		picking = false;
	}

	matsystemsurface->DrawFilledRect(x, y, basetab::tab_w, basetab::tab_h);

	matsystemsurface->SetTextColor(Color(186, 186, 186));
	matsystemsurface->SetFont(GetFont());

	int tw, th;
	matsystemsurface->GetTextSize(GetFont(), title, tw, th);

	matsystemsurface->SetTextPos(x + basetab::tab_title_left_pad, y + (basetab::tab_h / 2) - (th / 2));
	matsystemsurface->DrawPrintText(title);

	if (picking)
	{
		if (key != 0x0)
		{
			ref_to_setting[2] = key;

			key = 0x0;

			picking = false;
		}
	}

	int current_key = ref_to_setting[2];

	if (current_key == 0x0 && !picking)
	{
		std::string status = "not set";

		matsystemsurface->SetTextColor(Color(146, 146, 146, 215));

		matsystemsurface->GetTextSize(GetFont(), status, tw, th);

		matsystemsurface->SetTextPos(x + basetab::tab_w - basetab::tab_title_left_pad - tw, y + (basetab::tab_h / 2) - (th / 2));
		matsystemsurface->DrawPrintText(status);
	}
	else
	{
		std::string status;
		if (!picking)
		{
			status = VirtualKeyCodeToString(current_key);

			for (auto& ch : status)
				ch = std::tolower(ch);
		}
		else
		{
			matsystemsurface->SetTextColor(Color(146, 146, 146, 215));

			status = "press a key...";
		}

		matsystemsurface->GetTextSize(GetFont(), status, tw, th);

		matsystemsurface->SetTextPos(x + basetab::tab_w - basetab::tab_title_left_pad - tw, y + (basetab::tab_h / 2) - (th / 2));
		matsystemsurface->DrawPrintText(status);
	}
}

void KeyPickerTab::OnPressedRight()
{
	picking = true;
}

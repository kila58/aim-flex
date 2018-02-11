#pragma once

void ESP::PlayerWeapon()
{
	matsystemsurface->SetFont(font);

	auto weapon = p->GetWeapon();

	if (weapon)
	{
		int tw, th;

		std::string name = std::string(weapon->GetCSWpnData()->szClassName).substr(7);

		wchar_t wname[128];
		MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, wname, 128);

		matsystemsurface->GetTextSize(font, wname, tw, th);

		//if (player && antiaiminfo)
		//	matsystemsurface->SetTextPos(x + w - tw + 1, y + h + 1);
		//else
			matsystemsurface->SetTextPos(x + w / 2 - tw / 2, y + h + 1);

		matsystemsurface->DrawPrintText(wname, std::wcslen(wname));
	}
}

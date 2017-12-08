#include "esp.hpp"

#include "../../aim-flex.hpp"

unsigned long font;

void ESP::Init()
{
	matsystemsurface->SetFontGlyphSet(font = matsystemsurface->CreateFont(), "Arial", 16, 500, 0, 0, FONTFLAG_OUTLINE);
}

player_info_t info;

bool InvalidPlayer(int i, C_BaseEntity* p, C_BaseEntity* lp)
{
	if (!p)
		return true;

	if (!engineclient->GetPlayerInfo(i, &info))
		return true;

	if (p == lp)
		return true;

	if (p->IsDormant())
		return true;

	if (!p->IsAlive())
		return true;

	return false;
}

void ESP::Invoke()
{
	if (engineclient->IsInGame())
	{
		matsystemsurface->SetFont(font);
		matsystemsurface->SetTextColor(Color(255, 255, 255));

		auto lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());

		for (int i = 1; i <= globals->maxClients; i++)
		{
			auto p = entitylist->GetClientEntity(i);

			if (InvalidPlayer(i, p, lp))
				continue;

			Vector pos = p->GetAbsOrigin();

			Vector _bottom = (pos + Vector(0, 0, 1));
			Vector _top = (pos + Vector(0, 0, 72));

			Vector bottom, top;

			if (WorldToScreen(_bottom, bottom) && WorldToScreen(_top, top))
			{
				float h = (bottom.y - top.y);
				float width = (h / 4);

				float x = top.x - width;
				float y = top.y;
				float w = width * 2;

				if (p->GetTeam() == lp->GetTeam())
					matsystemsurface->SetDrawColor(Color(72, 133, 237));
				else
					matsystemsurface->SetDrawColor(Color(219, 50, 54));

				matsystemsurface->DrawOutlinedRect(x, y, w, h);

				matsystemsurface->SetDrawColor(Color(0, 0, 0));
				matsystemsurface->DrawOutlinedRect(x + 1, y + 1, w - 2, h - 2);
				matsystemsurface->DrawOutlinedRect(x - 1, y - 1, w + 2, h + 2);

				wchar_t wname[128];
				MultiByteToWideChar(CP_UTF8, 0, info.name, -1, wname, 128);

				int tw, th;
				matsystemsurface->GetTextSize(font, wname, tw, th);

				matsystemsurface->SetTextPos(x + w / 2 - tw / 2, y - th - 2);
				matsystemsurface->DrawPrintText(wname, std::wcslen(wname));

				auto weapon = p->GetWeapon();

				if (weapon)
				{
					std::string name = std::string(weapon->GetCSWpnData()->szClassName).substr(7);

					wchar_t wname[128];
					MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, wname, 128);

					matsystemsurface->GetTextSize(font, wname, tw, th);

					matsystemsurface->SetTextPos(x + w / 2 - tw / 2, y + h + 1);
					matsystemsurface->DrawPrintText(wname, std::wcslen(wname));
				}
			}
		}
	}
}

void ESP::Destroy()
{

}

extern ESP esp;

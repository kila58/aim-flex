#include "esp.hpp"

#include "../../aim-flex.hpp"

#include "../settings/settings.hpp"
#include "../playermanager/playermanager.hpp"

unsigned long font;

void ESP::Init()
{
	matsystemsurface->SetFontGlyphSet(font = matsystemsurface->CreateFont(), "Arial", 16, 500, 0, 0, FONTFLAG_OUTLINE);
}

player_info_t info;

bool InvalidPlayerESP(int i, C_BaseEntity* p, C_BaseEntity* lp)
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
	if (settings.Get<bool>("esp_enabled") && engineclient->IsInGame())
	{
		matsystemsurface->SetFont(font);
		matsystemsurface->SetTextColor(Color(255, 255, 255));

		C_BaseEntity* lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());

		for (int i = 1; i <= globals->maxClients; i++)
		{
			auto p = entitylist->GetClientEntity(i);

			if (InvalidPlayerESP(i, p, lp))
				continue;

			Vector pos = p->GetAbsOrigin();

			/*
			Vector mins = p->GetMins();
			Vector maxs = p->GetMaxs();

			Vector min, max;
			if (WorldToScreen(pos + mins, min) && WorldToScreen(pos + maxs, max))
			{
				matsystemsurface->SetDrawColor(Color(0, 0, 0));

				matsystemsurface->DrawOutlinedRect(min.x, min.y, 2, 2);

				matsystemsurface->DrawOutlinedRect(max.x, max.y, 2, 2);
			}
			*/

			Vector _bottom = (pos + Vector(0, 0, 1));
			Vector _top = (pos + Vector(0, 0, 72));

			Vector bottom, top;

			if (WorldToScreen(_bottom, bottom) && WorldToScreen(_top, top))
			{
				int h = ceil(bottom.y - top.y);
				int width = ceil(h / 4);

				int x = ceil(top.x - width);
				int y = ceil(top.y);
				int w = ceil(width * 2);

				if (p->GetTeam() == lp->GetTeam())
					matsystemsurface->SetDrawColor(Color(72, 133, 237));
				else
					matsystemsurface->SetDrawColor(Color(219, 50, 54));

				matsystemsurface->DrawOutlinedRect(x, y, w, h);

				matsystemsurface->SetDrawColor(Color(0, 0, 0));
				matsystemsurface->DrawOutlinedRect(x + 1, y + 1, w - 2, h - 2);
				matsystemsurface->DrawOutlinedRect(x - 1, y - 1, w + 2, h + 2);

				matsystemsurface->DrawFilledRect(x - 5, y - 1, 3, h + 2);

				int hp = p->GetHealth();
				int height = (int)((clamp((float)hp / 100.f, 0.f, 100.f)) * h);

				matsystemsurface->SetDrawColor(HSVtoRGB(hp / 100.f * 120.f, 1.f, 1.f));
				matsystemsurface->DrawFilledRect(x - 4, y + (h - height), 1, height);

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

		for (auto& p : playermanager.GetPlayers())
		{
			Vector lastscreen;
			bool prev = false;

			auto& first = p.backtrackinfo.ticks.back();
			auto& last = p.backtrackinfo.ticks.front();

			for (auto& tick : p.backtrackinfo.ticks)
			{
				Vector center = tick.head;
				Vector screen;

				if (WorldToScreen(center, screen) && prev)
				{
					matsystemsurface->SetDrawColor(Color(255, 255, 255));
					matsystemsurface->DrawLine(lastscreen.x, lastscreen.y, screen.x, screen.y);
				}

				lastscreen = screen;

				prev = true;
			}
		}
	}
}

void ESP::Destroy()
{

}

ESP esp;

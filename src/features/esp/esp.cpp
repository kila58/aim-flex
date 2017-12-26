#include "esp.hpp"

#include "../../aim-flex.hpp"

#include "../settings/settings.hpp"
#include "../playermanager/playermanager.hpp"

unsigned long font;
unsigned long font2;

void ESP::Init()
{
	matsystemsurface->SetFontGlyphSet(font = matsystemsurface->CreateFont(), "Arial", 16, 500, 0, 0, FONTFLAG_OUTLINE);
	matsystemsurface->SetFontGlyphSet(font2 = matsystemsurface->CreateFont(), "Arial", 12, 500, 0, 0, FONTFLAG_OUTLINE);
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
	bool antiaiminfo = settings.Get<bool>("esp_antiaiminfo");

	if (settings.Get<bool>("esp_enabled") && engineclient->IsInGame())
	{
		matsystemsurface->SetTextColor(Color(255, 255, 255));

		C_BaseEntity* lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());

		for (int i = 1; i <= globals->maxClients; i++)
		{
			auto p = entitylist->GetClientEntity(i);

			if (InvalidPlayerESP(i, p, lp))
				continue;

			auto& player = playermanager.GetPlayer(info.userID);

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
				matsystemsurface->SetFont(font);

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

				matsystemsurface->SetFont(font2);

				if (player && antiaiminfo)
				{
					int addy = 0;
				
					for (int i = 0; i < 3; i++)
					{
						float thing;

						if (i == 0)
							thing = p->LowerBodyYaw();
						else if (i == 1)
							thing = player.resolverinfo.eye.y;
						else if (i == 2)
						{
							float diff = p->LowerBodyYaw() - player.resolverinfo.eye.y;
							diff = clamp(normalize(diff), -180.f, 180.f);

							thing = std::abs(diff);
						}

						wchar_t wname[128];
						MultiByteToWideChar(CP_UTF8, 0, std::to_string((int)thing).c_str(), -1, wname, 128);

						matsystemsurface->SetTextPos(x + w + 3, y - 2 + addy);
						matsystemsurface->DrawPrintText(wname, std::wcslen(wname));

						matsystemsurface->GetTextSize(font, wname, tw, th);

						addy += (th / 2) + 2;
					}
				}

				matsystemsurface->SetFont(font);

				auto weapon = p->GetWeapon();

				if (weapon)
				{
					std::string name = std::string(weapon->GetCSWpnData()->szClassName).substr(7);

					wchar_t wname[128];
					MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, wname, 128);

					matsystemsurface->GetTextSize(font, wname, tw, th);

					if (player && antiaiminfo)
						matsystemsurface->SetTextPos(x + w - tw, y + h + 1);
					else
						matsystemsurface->SetTextPos(x + w / 2 - tw / 2, y + h + 1);

					matsystemsurface->DrawPrintText(wname, std::wcslen(wname));
				}
			}
		}

		for (auto& p : playermanager.GetPlayers())
		{
			Vector lastscreen;
			bool prev = false;

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

#include "esp.hpp"

#include "../../aim-flex.hpp"

#include "../settings/settings.hpp"
#include "../playermanager/playermanager.hpp"

#include "../aimbot/aimbot.hpp"

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

bool ESP::CreateBox(C_BaseEntity* p, int& x, int& y, int& w, int& h)
{
	static const Vector edges[4] =
	{
		Vector(1.0f, 1.0f, 1.0f),
		Vector(-1.0f, 1.0f, 1.0f),
		Vector(1.0f, -1.0f, 1.0f),
		Vector(-1.0f, -1.0f, 1.0f),
	};

	Vector origin = p->GetAbsOrigin();

	Angle rot = Angle(0, p->GetEyeAngle().y, 0);

	const model_t* model = p->GetModel();
	if (!model)
		return false;

	Vector mins, maxs;
	modelinfo->GetModelRenderBounds(model, mins, maxs);

	if (p->IsDucked())
		maxs.z -= 16.f;

	int screenw, screeh;
	engineclient->GetScreenSize(screenw, screeh);

	x = screenw;
	y = screeh;
	w = -screenw;
	h = -screeh;

	for (int i = 0; i < 4; i++)
	{
		Vector mins2d, maxs2d;

		if (!WorldToScreen(origin + (mins * edges[i].Rotate(rot)), mins2d))
			return false;

		if (!WorldToScreen(origin + (maxs * edges[i].Rotate(rot)), maxs2d))
			return false;

		x = std::min<int>(x, (int)ceil(std::min(mins2d.x, maxs2d.x)));
		y = std::min<int>(y, (int)ceil(std::min(mins2d.y, maxs2d.y)));
		w = std::max<int>(w, (int)ceil(std::max(mins2d.x, maxs2d.x)));
		h = std::max<int>(h, (int)ceil(std::max(mins2d.y, maxs2d.y)));
	}

	w -= x;
	h -= y;

	return true;
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

			int x, y, w, h;
			if (CreateBox(p, x, y, w, h))
			{
				matsystemsurface->SetFont(font);

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

					for (int i = 0; i < 2; i++)
					{
						matsystemsurface->SetTextPos(x + w + 3, y - 2 + addy);

						if (i == 0)
						{
							switch (player.resolverinfo.fakerecord.first)
							{
							case REAL:
								matsystemsurface->DrawPrintText(L"R", std::wcslen(L"R"));
								break;
							case FAKE:
								matsystemsurface->DrawPrintText(L"F", std::wcslen(L"F"));
								break;
							case UNDETERMINED:
								matsystemsurface->DrawPrintText(L"U", std::wcslen(L"U"));
								break;
							}
						}
						else if (i == 1)
						{
							auto size = player.backtrackinfo.ticks.size();

							wchar_t wname[128];
							MultiByteToWideChar(CP_UTF8, 0, std::to_string(size).c_str(), -1, wname, 128);

							matsystemsurface->SetTextPos(x + w + 3, y - 2 + addy);
							matsystemsurface->DrawPrintText(wname, std::wcslen(wname));
						}

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
						matsystemsurface->SetTextPos(x + w - tw + 1, y + h + 1);
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

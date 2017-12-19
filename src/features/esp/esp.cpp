#include "esp.hpp"

#include "../../aim-flex.hpp"

#include "../settings/settings.hpp"

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
	if (settings.Get<bool>("esp_enabled") && engineclient->IsInGame())
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

			studiohdr_t* hdr = modelinfo->GetStudiomodel(p->GetModel());

			VMatrix bones[128];
			if (!p->SetupBones(bones, lp->GetTickBase() * globals->interval_per_tick))
				continue;

			mstudiobbox_t* hitbox = hdr->GetHitbox(0, 0);

			if (!hitbox)
				continue;

			float mod = hitbox->m_flRadius != -1.f ? hitbox->m_flRadius : 0.f;

			Vector min, max, center;
			VectorTransform(hitbox->bbmin - Vector(mod, mod, mod), bones[hitbox->bone], min);
			VectorTransform(hitbox->bbmax + Vector(mod, mod, mod), bones[hitbox->bone], max);
			center = (min + max) * 0.5f;

			Vector screen;
			if (WorldToScreen(center, screen))
			{
				matsystemsurface->SetDrawColor(Color(255, 255, 255));

				matsystemsurface->DrawOutlinedRect(screen.x, screen.y, 2, 2);
			}

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

				matsystemsurface->DrawFilledRect(x - 5, y - 1, 3, h + 2);

				int hp = p->GetHealth();
				int height = (int)((clamp((float)hp / 100.f, 0.f, 100.f)) * (float)h);

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
	}
}

void ESP::Destroy()
{

}

ESP esp;

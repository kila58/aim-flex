#include "esp.hpp"

#include "../../aim-flex.hpp"

#include "../settings/settings.hpp"
#include "../playermanager/playermanager.hpp"
#include "../input/input.hpp"
#include "../prediction/prediction.hpp"

#include "../aimbot/aimbot.hpp"

#include "box/box.hpp"
#include "healthbar/healthbar.hpp"
#include "text/text.hpp"
#include "weapon/weapon.hpp"
#include "backtrack/backtrack.hpp"

void ESP::Init()
{
	matsystemsurface->SetFontGlyphSet(font = matsystemsurface->CreateFont(), "Arial", 16, 500, 0, 0, FONTFLAG_OUTLINE);
	matsystemsurface->SetFontGlyphSet(font2 = matsystemsurface->CreateFont(), "Arial", 12, 500, 0, 0, FONTFLAG_OUTLINE);
}

bool ESP::InvalidPlayerESP(int i, C_BaseEntity* p, C_BaseEntity* lp)
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
	if (input.KeyDownOnce(settings.Get<int>("esp_toggle_key")))
		settings.Set<bool>("esp_enabled", !settings.Get<bool>("esp_enabled"));

	lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());
	if (!lp)
		return;

	//if (AreArgumentsSet(CREATEMOVE))
	//	gamemove.RenderMove(lp, GetArg<CUserCmd*>(GetArguments(CREATEMOVE), 0), 8);

	bool antiaiminfo = settings.Get<bool>("esp_antiaiminfo");

	if (settings.Get<bool>("esp_enabled") && engineclient->IsInGame())
	{
		matsystemsurface->SetTextColor(Color(255, 255, 255));

		Vector lpeyepos = lp->GetAbsOrigin() + lp->GetEyeOffset();

		for (int i = 1; i <= globals->maxClients; i++)
		{
			p = entitylist->GetClientEntity(i);

			if (InvalidPlayerESP(i, p, lp))
				continue;

			auto& player = playermanager.GetPlayer(info.userID);

			if (CreateBox(p, x, y, w, h))
			{
				matsystemsurface->SetFont(font);

				PlayerBox();
				PlayerHealthBar();
				PlayerText();
				PlayerWeapon();
				PlayerBacktrack();
			}
		}
	}
}

void ESP::Destroy()
{

}

ESP esp;

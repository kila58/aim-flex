#include "esp.hpp"

#include "../../aim-flex.hpp"

void ESP::Init()
{

}

void ESP::Invoke()
{
	if (g_EngineClient->IsInGame())
	{
		for (int i = 1; i <= gpGlobals->maxClients; i++)
		{
			CBaseEntity* p = entitylist->GetClientEntity(i);

			if (!p)
				continue;

			player_info_t info;

			if (!g_EngineClient->GetPlayerInfo(i, &info))
				continue;

			Vector pos = p->GetAbsOrigin();
			Vector screen;

			if (WorldToScreen(pos, screen))
			{
				g_MatSystemSurface->SetDrawColor(Color(255, 0, 0));
				g_MatSystemSurface->DrawFilledRect((int)screen.x, (int)screen.y, 5, 5);
			}
		}
	}
}

void ESP::Destroy()
{

}

extern ESP esp;

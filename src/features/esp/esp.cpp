#include "esp.hpp"

#include "../../aim-flex.hpp"

void ESP::Init()
{

}

void ESP::Invoke()
{
	if (engineclient->IsInGame())
	{
		auto lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());

		for (int i = 1; i <= globals->maxClients; i++)
		{
			auto p = entitylist->GetClientEntity(i);

			if (!p)
				continue;

			if (p == lp)
				continue;

			player_info_t info;

			if (!engineclient->GetPlayerInfo(i, &info))
				continue;

			Vector pos = p->GetAbsOrigin();
			Vector screen;

			if (WorldToScreen(pos, screen))
			{
				matsystemsurface->SetDrawColor(Color(255, 0, 0));
				matsystemsurface->DrawFilledRect((int)screen.x, (int)screen.y, 5, 5);
			}
		}
	}
}

void ESP::Destroy()
{

}

extern ESP esp;

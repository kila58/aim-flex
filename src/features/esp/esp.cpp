#include "esp.hpp"

#include "../../aim-flex.hpp"

void ESP::Init()
{

}

bool InvalidPlayer(int i, CBaseEntity* p, CBaseEntity* lp)
{
	if (!p)
		return true;

	static player_info_t info;

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
					matsystemsurface->SetDrawColor(Color(72, 133, 237, 235));
				else
					matsystemsurface->SetDrawColor(Color(219, 50, 54, 235));

				matsystemsurface->DrawOutlinedRect(x, y, w, h);

				matsystemsurface->SetDrawColor(Color(0, 0, 0, 235));
				matsystemsurface->DrawOutlinedRect(x + 1, y + 1, w - 2, h - 2);
				matsystemsurface->DrawOutlinedRect(x - 1, y - 1, w + 2, h + 2);
			}
		}
	}
}

void ESP::Destroy()
{

}

extern ESP esp;

#include "rage.hpp"

#include "../../aim-flex.hpp"

#include "../playermanager/playermanager.hpp"
#include "../aimbot/aimbot.hpp"
#include "../prediction/prediction.hpp"

void Rage::Init()
{

}

bool FindTarget(CUserCmd* cmd, Angle& ang)
{
	for (auto& target : playermanager.GetPlayers())
	{
		C_BaseEntity* p = target.ent;

		if (target.backtrackinfo.ticks.size() > 1)
		{
			auto& first = target.backtrackinfo.ticks.back();
			auto& last = target.backtrackinfo.ticks.front();

			if (!first.head.IsZero() && !last.head.IsZero())
			{
				if (!aimbot.IsVisible(p, first.head) && aimbot.IsVisible(p, last.head))
				{
					aimbot.CalculateAngle(last.head, ang);
					backtrack.BacktrackToTick(cmd, last);

					return true;
				}
			}
		}

		/*
		Vector pos = aimbot.GetHitbox(p, 0);
		
		if (!pos.IsZero() && (aimbot.IsVisible(p, pos)))
		{
			aimbot.CalculateAngle(pos, ang);

			return true;
		}
		*/
	}

	return false;
}

void Rage::Invoke()
{
	C_BaseEntity* lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());
	auto cmd = GetArg<CUserCmd*>(GetArguments(CREATEMOVE), 0);

	Angle ang;
	if (lp->IsAlive() && aimbot.CanShoot() && FindTarget(cmd, ang))
	{
		cmd->viewangles = ang;
		aimbot.NoRecoil();

		cmd->buttons |= IN_ATTACK;

		aimbot.MovementFix();
	}
}

void Rage::Destroy()
{

}

Rage rage;

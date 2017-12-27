#include "rage.hpp"

#include "../../aim-flex.hpp"

#include "../playermanager/playermanager.hpp"
#include "../aimbot/aimbot.hpp"
#include "../prediction/prediction.hpp"
#include "../antiaim/antiaim.hpp"

void Rage::Init()
{

}

bool FindTarget(CUserCmd* cmd, Angle& ang)
{
	for (auto& target : playermanager.GetPlayers())
	{
		C_BaseEntity* p = target.ent;

		for (auto& tick : target.backtrackinfo.ticks)
		{
			if (aimbot.IsVisible(p, tick.head))
			{
				aimbot.CalculateAngle(tick.head, ang);
				backtrack.BacktrackToTick(cmd, tick);

				return true;
			}
		}

		/*
		Hitboxes hitboxes;
		if (aimbot.GetHitboxes(p, hitboxes))
		{
			for (auto& hitbox : hitboxes)
			{
				Vector& pos = hitbox.center;

				if (aimbot.IsVisible(p, pos))
				{
					aimbot.CalculateAngle(pos, ang);

					return true;
				}
			}
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
	if (aimbot.CanShoot() && FindTarget(cmd, ang))
	{
		cmd->viewangles = ang;
		aimbot.NoRecoil();

		cmd->buttons |= IN_ATTACK;

		aimbot.MovementFix();
	}
	else
	{
		antiaim.Invoke();

		aimbot.MovementFix();
	}
}

void Rage::Destroy()
{

}

Rage rage;

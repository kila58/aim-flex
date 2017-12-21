#include "rage.hpp"

#include "../../aim-flex.hpp"

#include "../playermanager/playermanager.hpp"
#include "../aimbot/aimbot.hpp"
#include "../prediction/prediction.hpp"

void Rage::Init()
{

}

bool FindTarget(Angle& ang)
{
	auto& targets = playermanager.GetPlayers();

	for (auto& target : targets)
	{
		C_BaseEntity* p = target.ent;
		Vector pos = aimbot.GetHitbox(p, 0);
		
		if (!pos.IsZero() && (aimbot.IsVisible(p, pos)))
		{
			aimbot.CalculateAngle(pos, ang);

			return true;
		}
	}

	return false;
}

void Rage::Invoke()
{
	C_BaseEntity* lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());
	auto cmd = GetArg<CUserCmd*>(GetArguments(CREATEMOVE), 0);

	Angle ang;
	if (aimbot.CanShoot() && FindTarget(ang))
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

#include "rage.hpp"

#include "../../aim-flex.hpp"

#include "../playermanager/playermanager.hpp"
#include "../aimbot/aimbot.hpp"

void Rage::Init()
{

}

bool FindTarget(Angle& ang)
{
	C_BaseEntity* lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());
	auto& targets = playermanager.GetPlayers();

	for (auto& target : targets)
	{
		C_BaseEntity* p = target.ent;
		Vector& center = aimbot.GetHitbox(p, 0);

		if (!center.IsZero() && aimbot.IsVisible(p, center))
		{
			aimbot.CalculateAngle(center, ang);

			ang -= (lp->GetAimPunch() * 2);
			ang.r = 0.f;

			return true;
		}
	}

	return false;
}

void Rage::Invoke()
{
	auto cmd = GetArg<CUserCmd*>(GetArguments(CREATEMOVE), 0);

	//cvar->ConsoleColorPrintf(std::to_string(aimbot.CanShoot()) + "\n");

	Angle ang;
	if (aimbot.CanShoot() && FindTarget(ang))
	{
		cmd->viewangles = ang;
		cmd->buttons |= IN_ATTACK;
	}
}

void Rage::Destroy()
{

}

Rage rage;

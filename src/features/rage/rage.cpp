#include "rage.hpp"

#include "../../aim-flex.hpp"

#include "../playermanager/playermanager.hpp"
#include "../aimbot/aimbot.hpp"

void Rage::Init()
{

}

bool FindTarget(Angle& ang)
{
	auto lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());
	Vector lpeyepos = lp->GetEyeOffset() + lp->GetAbsOrigin();

	auto& targets = playermanager.GetPlayers();

	for (auto& target : targets)
	{
		C_BaseEntity* p = target.ent;
		Vector& center = aimbot.GetHitbox(p, 0);

		VectorAngles(center - lpeyepos, ang);
		ang = ang - (lp->GetAimPunch() * 2);

		return true;
	}

	return false;
}

void Rage::Invoke()
{
	auto cmd = GetArg<CUserCmd*>(GetArguments(CREATEMOVE), 0);

	Angle ang;
	if (FindTarget(ang))
		cmd->viewangles = ang;
}

void Rage::Destroy()
{

}

Rage rage;

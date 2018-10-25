#include "triggerbot.hpp"

#include "../../aim-flex.hpp"

#include "../playermanager/playermanager.hpp"
#include "../input/input.hpp"
#include "../settings/settings.hpp"

#include "../aimbot/aimbot.hpp"

void Triggerbot::Init()
{

}

void Triggerbot::Invoke()
{
	if (true)
		return;

	if (!input.KeyDown(settings.Get<int>("legit_aimkey")))
		return;

	auto cmd = GetArg<CUserCmd*>(GetArguments(), 0);

	C_BaseEntity* lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());
	//if (true)
	if (!lp || !lp->IsAlive())
		return;

	Vector lpeyepos = lp->GetEyeOffset() + lp->GetAbsOrigin();
	auto weapon = lp->GetWeapon();
	if (!weapon)
		return;

	Vector forward, right, up;
	AngleVectors(cmd->viewangles, forward, right, up);

	//float range = weapon->GetCSWpnData()->m_flRange;
	float range = 1;
	Vector end = lpeyepos + (forward * range);

	static CTraceFilterSkipTwoEntities filter;
	static trace_t trace;
	static Ray_t ray;

	ray.Init(lpeyepos, end);
	filter.pSkip1 = lp;

	filter.type = TRACE_WORLD_ONLY;
	enginetrace->TraceRay(ray, 0x46004003, &filter, &trace);

	Vector traceend = lpeyepos + forward;

	//debug << traceend.x << ", " << traceend.y << ", " << traceend.z << "\n";

	for (auto& ply : playermanager.GetPlayers())
	{
		for (auto& tick : ply.backtrackinfo.ticks)
		//auto& tick = ply.backtrackinfo.ticks.front();
		{
			if (DoesIntersectCapsule(lpeyepos, forward, tick.hitboxinfo.minsnoradius, tick.hitboxinfo.maxsnoradius, tick.hitboxinfo.radius, 10))
			{
				backtrack.BacktrackToTick(cmd, tick);

				debug << ply.ent->GetName() << "\n";

				if (aimbot.CanShoot())
					cmd->buttons |= IN_ATTACK;
			}
		}
	}
}

void Triggerbot::Destroy()
{

}

Triggerbot triggerbot;

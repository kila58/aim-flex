#include "backtrack.hpp"

#include "../../aim-flex.hpp"

#include "../playermanager/playermanager.hpp"
#include "../aimbot/aimbot.hpp"
#include "../prediction/prediction.hpp"

void Backtrack::Init()
{

}

float Backtrack::GetServerTickCount()
{
	auto nci = engineclient->GetNetChannelInfo();

	float outgoing = 0.f;
	float incoming = 0.f;

	if (nci)
		outgoing = nci->GetLatency(FLOW_OUTGOING), incoming = nci->GetLatency(FLOW_INCOMING);

	//return floor(TIME_TO_TICKS(outgoing + incoming)) + /*TIME_TO_TICKS(globals->curtime)*/globals->tickcount + 1;
	return floor(TIME_TO_TICKS(outgoing + incoming)) + TIME_TO_TICKS(globals->curtime) + 1;
}

float Backtrack::GetLerpTime()
{
	float interp = cvar->FindVar("cl_interp")->value<float>();

	float updaterate = cvar->FindVar("cl_updaterate")->value<float>();
	float minupdaterate = cvar->FindVar("sv_minupdaterate")->value<float>();
	float maxupdaterate = cvar->FindVar("sv_maxupdaterate")->value<float>();

	float interp_ratio = cvar->FindVar("cl_interp_ratio")->value<float>();
	float min_interp_ratio = cvar->FindVar("sv_client_min_interp_ratio")->value<float>();
	float max_interp_ratio = cvar->FindVar("sv_client_max_interp_ratio")->value<float>();

	if (min_interp_ratio > interp_ratio)
		interp_ratio = min_interp_ratio;

	if (interp_ratio > max_interp_ratio)
		interp_ratio = max_interp_ratio;

	if (maxupdaterate <= updaterate)
		updaterate = maxupdaterate;

	if (minupdaterate > updaterate)
		updaterate = minupdaterate;

	float result = interp_ratio / updaterate;

	if (result <= interp)
		result = interp;

	return result;
}

void Backtrack::Invoke()
{
	int stage = GetArg<int>(GetArguments(), 0);
	if (stage != FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		return;

	float time = GetServerTickCount();
	float lerp = GetLerpTime();

	for (auto& target : playermanager.GetPlayers())
	{
		//cvar->ConsoleColorPrintf(target.ent->GetName() + ": " + std::to_string(target.ent->GetAbsAngles().y - target.ent->GetEyeAngle().y) + "\n");

		// todo: account for ping again
		std::experimental::erase_if(target.backtrackinfo.ticks, [&time, &lerp](const Tick& tick)
		{ 
			return (((globals->curtime + lerp) - TICKS_TO_TIME(tick.tickcount)) > 0.2f);
		});

		C_BaseEntity* p = target.ent;

		Vector head = aimbot.GetHitbox(p, 0, false);

		if (!head.IsZero())
			target.backtrackinfo.ticks.emplace_back(p->GetSimulationTime(), globals->curtime, head, globals->tickcount);
	}
}

void Backtrack::BacktrackToTick(CUserCmd* cmd, const Tick& tick)
{
	cmd->tick_count = TIME_TO_TICKS(tick.time + GetLerpTime());
}

void Backtrack::Destroy()
{

}

static Tick badtick;
Tick& BacktrackInfo::FindTick(C_BaseEntity* p, float simulation)
{
	auto& player = playermanager.GetPlayer(p);
	if (!player)
		return badtick;

	auto& ticks = player.backtrackinfo.ticks;
	if (ticks.empty())
		return badtick;

	auto it = std::find(ticks.begin(), ticks.end(), simulation);

	if (it == ticks.end())
		return badtick;

	return ticks[std::distance(ticks.begin(), it)];
}

Backtrack backtrack;

#include "backtrack.hpp"

#include "../../aim-flex.hpp"

#include "../playermanager/playermanager.hpp"
#include "../aimbot/aimbot.hpp"

void Backtrack::Init()
{

}

float Backtrack::GetServerTickCount()
{
	auto nci = engineclient->GetNetChannelInfo();

	float outgoing = 0.f;
	//float incoming = 0.f;

	if (nci)
		outgoing = nci->GetAvgLatency(FLOW_OUTGOING)/*, incoming = nci->GetAvgLatency(FLOW_INCOMING)*/;

	return floor(TIME_TO_TICKS(outgoing)) + globals->tickcount + 1;
}

float Backtrack::GetLerpTime()
{
	float interp = cvar->FindVar("cl_interp")->xor_value_float();

	float updaterate = cvar->FindVar("cl_updaterate")->xor_value_float();
	float min_updaterate = cvar->FindVar("sv_minupdaterate")->xor_value_float();
	float max_updaterate = cvar->FindVar("sv_maxupdaterate")->xor_value_float();

	float interp_ratio = cvar->FindVar("cl_interp_ratio")->xor_value_float();
	float min_interp_ratio = cvar->FindVar("sv_client_min_interp_ratio")->xor_value_float();
	float max_interp_ratio = cvar->FindVar("sv_client_max_interp_ratio")->xor_value_float();

	if (min_interp_ratio > interp_ratio)
		interp_ratio = min_interp_ratio;

	if (interp_ratio > max_interp_ratio)
		interp_ratio = max_interp_ratio;

	if (max_updaterate <= updaterate)
		updaterate = max_updaterate;

	if (min_updaterate > updaterate)
		updaterate = min_updaterate;

	float result = interp_ratio / updaterate;

	if (result <= interp)
		result = interp;

	return result;
}

void Backtrack::Invoke()
{
	float time = GetServerTickCount();
	float lerp = GetLerpTime();

	for (auto& target : playermanager.GetPlayers())
	{
		std::experimental::erase_if(target.backtrackinfo.ticks, [&time, &lerp](const Tick& tick)
		{
			return (((TICKS_TO_TIME(time) + lerp) - tick.time) > 0.2f);
		});

		C_BaseEntity* p = target.ent;

		Vector head = aimbot.GetHitbox(p, 0);
		head -= p->GetAbsOrigin();
		head += p->GetOrigin();

		target.backtrackinfo.ticks.emplace_back(p->GetSimulationTime(), head);
	}
}

void Backtrack::BacktrackToTick(CUserCmd* cmd, const Tick& tick)
{
	cmd->tick_count = TIME_TO_TICKS(tick.time + GetLerpTime());
}

void Backtrack::Destroy()
{

}

Backtrack backtrack;

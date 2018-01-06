#include "backtrack.hpp"

#include "../../aim-flex.hpp"

#include "../playermanager/playermanager.hpp"
#include "../aimbot/aimbot.hpp"
#include "../prediction/prediction.hpp"

void Backtrack::Init()
{

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

bool Backtrack::TickIsValid(const Tick& tick, float lerp)
{
	float correct = 0;

	correct += engineclient->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING);
	correct += engineclient->GetNetChannelInfo()->GetLatency(FLOW_INCOMING);
	correct += lerp;

	float sv_maxunlag = cvar->FindVar("sv_maxunlag")->value<float>();
	correct = clamp(correct, 0.f, sv_maxunlag);

	float delta = correct - (predict.pred_time - tick.time);

	return !(fabsf(delta) > 0.2f);
}

void Backtrack::Invoke()
{
	auto lerp = GetLerpTime();

	for (auto& target : playermanager.GetPlayers())
	{
		//cvar->ConsoleColorPrintf(target.ent->GetName() + ": " + std::to_string(target.ent->GetAbsAngles().y - target.ent->GetEyeAngle().y) + "\n");

		std::experimental::erase_if(target.backtrackinfo.ticks, [this, &lerp](const Tick& tick)
		{ 
			return !TickIsValid(tick, lerp);
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

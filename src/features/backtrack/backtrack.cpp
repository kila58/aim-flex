#include "backtrack.hpp"

#include "../../aim-flex.hpp"

#include "../playermanager/playermanager.hpp"
#include "../aimbot/aimbot.hpp"
#include "../prediction/prediction.hpp"

using CreateAnimState_t = void(__thiscall*)(CCSGOPlayerAnimState*, C_BaseEntity*);
void Animations::CreateAnimationState(CCSGOPlayerAnimState* state, C_BaseEntity* player)
{
	static auto CreateAnimState = (CreateAnimState_t)SigScan("55 8B EC 56 8B F1 B9 ? ? ? ? C7 46", "client.dll");
	if (!CreateAnimState)
		return;

	CreateAnimState(state, player);
}

typedef void(__vectorcall *fnUpdateAnimState)(PVOID, PVOID, float, float, float, PVOID);
void Animations::UpdateAnimationState(CCSGOPlayerAnimState* state, Angle ang)
{
	static auto UpdateAnimState = (fnUpdateAnimState)SigScan("55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24", "client.dll");
	if (!UpdateAnimState)
		return;

	UpdateAnimState(state, NULL, NULL, ang.y, ang.p, NULL);
}

void Animations::UpdateServerAnimations()
{
	for (Player& player : playermanager.GetPlayers())
	{
		C_BaseEntity* p = player.ent;

		if (!player.dormantplayer->animstate)
		{
			std::unique_ptr<CCSGOPlayerAnimState, AnimStateDeleter> memory((CCSGOPlayerAnimState*)g_pMemAlloc->Alloc(sizeof(CCSGOPlayerAnimState)));
			player.dormantplayer->animstate = std::move(memory);

			if (player.dormantplayer->animstate.get() != nullptr)
			{
				CreateAnimationState(player.dormantplayer->animstate.get(), p);
			}
		}
		else
		{
			Vector backup_absorigin = p->GetAbsOrigin();
			Angle backup_absangles = p->GetAbsAngles();

			auto backup_poses = p->GetPoseParameters();
			auto backup_layers = p->GetAnimLayers();

			UpdateAnimationState(player.dormantplayer->animstate.get(), player.resolverinfo.eye);

			player.poses = p->GetPoseParameters();
			player.animationlayers = p->GetAnimLayers();

			p->SetPoseParameters(backup_poses);
			p->SetAnimLayers(backup_layers);

			p->SetAbsOrigin(backup_absorigin);
			p->SetAbsAngles(backup_absangles);
		}
	}
}

void Animations::PVSFix()
{
	for (int i = 1; i <= globals->maxClients; i++)
	{
		if (i == engineclient->GetLocalPlayer())
			continue;

		auto p = entitylist->GetClientEntity(i);
		if (!p) 
			continue;

		p->ClearOcclusionFlags();
	}
}

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

	float delta = correct - (globals->curtime - tick.time);

	return !(fabsf(delta) > 0.2f);
}

void Backtrack::Invoke()
{
	auto lerp = GetLerpTime();

	for (auto& target : playermanager.GetPlayers())
	{
		std::experimental::erase_if(target.backtrackinfo.ticks, [this, &lerp](const Tick& tick)
		{ 
			return !TickIsValid(tick, lerp);
		});

		C_BaseEntity* p = target.ent;

		HitboxInfo info;
		info.head = aimbot.GetHitbox(p, 0, false);
		aimbot.GetHitboxBounds(info.mins, info.maxs);
		info.radius = aimbot.GetRadius();

		if (!info.head.IsZero())
			target.backtrackinfo.ticks.emplace_back(p->GetSimulationTime(), info);
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
Animations animations;

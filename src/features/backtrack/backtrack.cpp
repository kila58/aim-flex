#include "backtrack.hpp"

#include "../../aim-flex.hpp"

#include "../playermanager/playermanager.hpp"
#include "../aimbot/aimbot.hpp"
#include "../prediction/prediction.hpp"

#include <random>

using CreateAnimStateType = void(__thiscall*)(CCSGOPlayerAnimState*, C_BaseEntity*);
void Animations::CreateAnimationState(CCSGOPlayerAnimState* state, C_BaseEntity* player)
{
	static auto CreateAnimState = (CreateAnimStateType)SigScan("55 8B EC 56 8B F1 B9 ? ? ? ? C7 46", "client_panorama.dll");
	if (!CreateAnimState)
	{
		debug << "!CreateAnimState\n";

		return;
	}

	CreateAnimState(state, player);
}

using UpdateAnimationStateType = void(__vectorcall*)(void*, void*, float, float, float, void*);
void Animations::UpdateAnimationState(CCSGOPlayerAnimState* state, Angle ang)
{
	static auto UpdateAnimState = (UpdateAnimationStateType)SigScan("55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24", "client_panorama.dll");
	if (!UpdateAnimState)
	{
		debug << "!UpdateAnimState\n";

		return;
	}

	if (!state)
		MessageBoxA(NULL, "commit food eat", "commit food eat", NULL);

	__asm
	{
		mov ecx, state

		movss xmm1, dword ptr[ang + 4]
		movss xmm2, dword ptr[ang]

		call UpdateAnimState
	}

	//UpdateAnimState(state, NULL, NULL, ang.y, ang.p, NULL);
}

// temporary solution, need to fix UpdateServerAnimations. this is using interpolated animations i think so bad.
void Animations::FixAnimations()
{
	for (Player& player : playermanager.GetPlayers())
	{
		C_BaseEntity* p = player.ent;

		if (!player.dormantplayer)
		{
			if (p)
				cvar->ConsoleColorPrintf(std::string(p->GetName()) + ": is bad" + "\n");

			continue;
		}

		if (!(p && p->IsAlive()))
			continue;

		//Vector backup_absorigin = p->GetAbsOrigin();
		//if (backup_absorigin.IsZero())
		//	continue;

		//Angle backup_absangles = p->GetAbsAngles();

		auto backup_poses = p->GetPoseParameters();
		auto backup_layers = p->GetAnimLayers();

		//p->SetAbsOrigin(p->GetOrigin());
		//p->SetAbsAngles(player.resolverinfo.absang);

		// hey nOX FIX THIS
		//p->UpdateClientSideAnimation();

		player.poses = p->GetPoseParameters();
		player.animationlayers = p->GetAnimLayers();

		p->SetPoseParameters(backup_poses);
		p->SetAnimLayers(backup_layers);

		//p->SetAbsOrigin(backup_absorigin);
		//p->SetAbsAngles(backup_absangles);

		//p->UpdateClientSideAnimation();

		/*
		float backup_curtime = globals->curtime;
		float backup_frametime = globals->frametime;

		//static auto host_timescale = cvar->FindVar("host_timescale");
		float host_timescale = cvar->FindVar("host_timescale")->value<float>();

		globals->curtime = p->GetOldSimulationTime() + globals->interval_per_tick;
		globals->frametime = globals->interval_per_tick * host_timescale;

		CCSGOPlayerAnimState* state = p->GetAnimationState();
		if (state)
			state->m_iLastClientSideAnimationUpdateFramecount = globals->framecount - 1;

		Vector backup_absorigin = p->GetAbsOrigin();
		Angle backup_absangles = p->GetAbsAngles();

		auto backup_poses = p->GetPoseParameters();
		auto backup_layers = p->GetAnimLayers();

		p->SetAbsOrigin(p->GetOrigin());
		p->SetAbsAngles(player.resolverinfo.absang);

		p->SetClientSideAnimation(true);
		
		p->UpdateClientSideAnimation();

		player.poses = p->GetPoseParameters();
		player.animationlayers = p->GetAnimLayers();

		p->SetClientSideAnimation(false);

		// restore
		p->SetPoseParameters(backup_poses);
		p->SetAnimLayers(backup_layers);

		p->SetAbsOrigin(backup_absorigin);
		p->SetAbsAngles(backup_absangles);

		globals->curtime = backup_curtime;
		globals->frametime = backup_frametime;
		*/
	}
}

void Animations::UpdateServerAnimations()
{
	for (Player& player : playermanager.GetPlayers())
	{
		C_BaseEntity* p = player.ent;

		if (!player.dormantplayer)
		{
			if (p)
				cvar->ConsoleColorPrintf(std::string(p->GetName()) + ": is bad" + "\n");

			continue;
		}

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

			p->SetAbsOrigin(p->GetOrigin());
			p->SetAbsAngles(player.resolverinfo.absang);

			// note: you have to force eye angle or else some of the poses will start lerping between invalid angles
			// another possible problem, when they are faking lby their body might be rotated in the wrong direction, making the head off by a little
			// so maybe force lby (feet goal in animstate) in some situations?
			//player.dormantplayer->animstate.get()->m_flGoalFeetYaw = p->GetEyeAngle().y;
			//player.dormantplayer->animstate.get()->m_flCurrentFeetYaw = p->GetEyeAngle().y; //(still rotates, breaks even more)

			UpdateAnimationState(player.dormantplayer->animstate.get(), p->GetEyeAngle());
			//cvar->ConsoleColorPrintf(std::to_string(p->GetEyeAngle().y) + "\n");
			
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
	correct = std::clamp(correct, 0.f, sv_maxunlag);

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
		aimbot.GetHitboxBoundsNoRadius(info.minsnoradius, info.maxsnoradius);
		info.radius = aimbot.GetRadius();
		info.hitbox = aimbot.GetHitboxIndex();

		if (!info.head.IsZero())
			target.backtrackinfo.ticks.emplace_back(p->GetSimulationTime(), info, p->GetAbsOrigin(), p->GetAbsAngles());
	}
}

void Backtrack::BacktrackToTick(CUserCmd* cmd, const Tick& tick)
{
	cmd->tick_count = TIME_TO_TICKS(tick.time + GetLerpTime());
}

void Backtrack::BulletImpact(Vector pos, C_BaseEntity* player)
{
	C_BaseEntity* lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());

	if (lp && player == lp && aimbot.target)
	{
		auto& target = playermanager.GetPlayer(aimbot.target);
		auto& info = target.resolverinfo;
	}
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

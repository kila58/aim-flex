#include "rage.hpp"

#include "../../aim-flex.hpp"

void Rage::Init()
{

}

struct Target
{
	Target(C_BaseEntity* ent, float distance) : ent(ent), distance(distance) {}

	C_BaseEntity* ent;
	float distance;
};

bool InvalidTarget(int i, C_BaseEntity* p, C_BaseEntity* lp)
{
	if (!p)
		return true;

	static player_info_t info;

	if (!engineclient->GetPlayerInfo(i, &info))
		return true;

	if (p == lp)
		return true;

	if (p->IsDormant())
		return true;

	if (!p->IsAlive())
		return true;

	if (p->GetTeam() == lp->GetTeam())
		return true;

	return false;
}

bool FindTarget(Angle& ang)
{
	std::deque<Target> targets;

	auto lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());
	Vector lporigin = lp->GetAbsOrigin();
	Vector lpeyepos = lp->GetEyeOffset() + lporigin;

	for (int i = 1; i <= globals->maxClients; i++)
	{
		auto p = entitylist->GetClientEntity(i);

		// todo: maybe use Distance2DSqr? (it will produce massive numbers, needs benchmark)
		if (!InvalidTarget(i, p, lp))
			targets.emplace_back(p, lporigin.Distance(p->GetAbsOrigin()));
	}

	std::sort(targets.begin(), targets.end(), [](const Target& a, const Target& b)
	{
		return a.distance < b.distance;
	});

	VMatrix bones[128];
	float curtime = lp->GetTickBase() * globals->interval_per_tick;

	for (auto& target : targets)
	{
		C_BaseEntity* p = target.ent;

		studiohdr_t* hdr = modelinfo->GetStudiomodel(p->GetModel());

		float curtime = lp->GetTickBase() * globals->interval_per_tick;

		VMatrix bones[128];
		if (!p->SetupBones(bones, curtime))
			continue;

		mstudiobbox_t* hitbox = hdr->GetHitbox(0, 0);

		if (!hitbox)
			continue;

		float mod = hitbox->m_flRadius != -1.f ? hitbox->m_flRadius : 0.f;

		Vector min, max, center;
		VectorTransform(hitbox->bbmin - Vector(mod, mod, mod), bones[hitbox->bone], min);
		VectorTransform(hitbox->bbmax + Vector(mod, mod, mod), bones[hitbox->bone], max);
		center = (min + max) * 0.5f;

		VectorAngles(center - lpeyepos, ang);

		return true;
	}

	return false;
}

void Rage::Invoke()
{
	auto cmd = GetArg<CUserCmd*>(GetArguments(), 0);

	Angle ang;
	if (FindTarget(ang))
		cmd->viewangles = ang;
}

void Rage::Destroy()
{

}

Rage rage;

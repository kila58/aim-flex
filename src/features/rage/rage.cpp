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

		if (!p->SetupBones(bones, curtime))
			continue;

		studiohdr_t* hdr =  modelinfo->GetStudiomodel(p->GetModel());
		mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

		mstudiobbox_t* hitbox = set->GetHitbox(0);

		if (!hitbox)
			continue;

		Vector vMin, vMax, vCenter, sCenter;
		VectorTransform(hitbox->bbmin, bones[hitbox->bone], vMin);
		VectorTransform(hitbox->bbmax, bones[hitbox->bone], vMax);
		vCenter = (vMin + vMax) * 0.5f;

		cvar->ConsoleColorPrintf(
			std::to_string(int((ceil(vCenter.y * 100.f)) / 100.f)) + ", " +
			std::to_string(int((ceil(vCenter.z * 100.f)) / 100.f)) + ", " +
			std::to_string(int((ceil(vCenter.x * 100.f)) / 100.f)) + "\n");

		Vector forward = Vector(vCenter.y, vCenter.z, vCenter.x) - lpeyepos;

		VectorAngles(forward, ang);

		return true;

		//auto model = p->GetModel();
		//studiohdr_t* hdr = modelinfo->GetStudiomodel(model);

		//if (!hdr)
		//	continue;

		//mstudiohitboxset_t* set = hdr->hitboxset(0);

		//if (!set)
		//	continue;

		//for (int h = 0; h < set->numhitboxes; h++)
		//{
			//auto hitbox = set->hitbox(0);

			//if (!hitbox/* && hitbox->radius*/)
			//	continue;

			//Vector min, max;
			//float mod = hitbox->radius != -1.f ? hitbox->radius : 0.f;

			//VectorTransform(hitbox->bbmin - Vector(mod, mod, 0), bones[hitbox->bone], min);
			//VectorTransform(hitbox->bbmax + Vector(mod, mod, 0), bones[hitbox->bone], max);

			//Vector forward = ((min + max) * 0.5f) - lpeyepos;

			//VectorAngles(forward, ang);

		//	return true;
		//}
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

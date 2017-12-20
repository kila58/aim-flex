#include "aimbot.hpp"

#include "../../aim-flex.hpp"

#include "../rage/rage.hpp"
#include "../legit/legit.hpp"

void Aimbot::Init()
{

}

void Aimbot::Invoke()
{
	rage.Invoke();

	lastplayer = nullptr;
	lp = nullptr;
}

static Vector empty = Vector();
Vector Aimbot::GetHitbox(C_BaseEntity* p, int hitboxindex)
{
	model_t* model = p->GetModel();
	if (!model)
		return empty;

	studiohdr_t* hdr = modelinfo->GetStudiomodel(model);
	if (!hdr)
		return empty;

	mstudiobbox_t* hitbox = hdr->GetHitbox(hitboxindex, 0);
	if (!hitbox)
		return empty;

	static VMatrix bones[128];

	if (lastplayer != p)
	{
		if (!p->SetupBones(bones))
			return empty;
	}

	float mod = hitbox->m_flRadius != -1.f ? hitbox->m_flRadius : 0.f;

	Vector min, max;
	VectorTransform(hitbox->bbmin - mod, bones[hitbox->bone], min);
	VectorTransform(hitbox->bbmax + mod, bones[hitbox->bone], max);

	lastplayer = p;

	return ((min + max) * 0.5f);
}

void Aimbot::CalculateAngle(const Vector& pos, Angle& out)
{
	if (!lp)
	{
		lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());
		lpeyepos = lp->GetEyeOffset() + lp->GetAbsOrigin();
	}
	
	VectorAngles(pos - lpeyepos, out);
}

bool Aimbot::IsVisible(C_BaseEntity* p, const Vector& pos)
{
	if (!lp)
	{
		lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());
		lpeyepos = lp->GetEyeOffset() + lp->GetAbsOrigin();
	}

	static CTraceFilterDouble filter;
	static trace_t tr;
	static Ray_t ray;

	ray.Init(lpeyepos, pos);
	filter.pSkip1 = lp;
	filter.pSkip2 = p;
	enginetrace->TraceRay(ray, 0x46004003, &filter, &tr);

	return (tr.m_pEnt == p || tr.fraction == 1);
}

bool Aimbot::CanShoot()
{
	static C_BaseCombatWeapon* weapon;

	if (!lp)
	{
		lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());
		lpeyepos = lp->GetEyeOffset() + lp->GetAbsOrigin();
	}

	if (!weapon)
	{
		if (!lp->IsAlive() || !(weapon = lp->GetWeapon()))
		{
			weapon = nullptr;

			return false;
		}
	}

	if (!(weapon->GetAmmo() > 0))
		return false;
	
	return lp->GetNextPrimaryAttack(weapon) <= globals->curtime;
}

void Aimbot::Destroy()
{

}

Aimbot aimbot;

#include "aimbot.hpp"

#include "../../aim-flex.hpp"

#include "../rage/rage.hpp"
#include "../legit/legit.hpp"
#include "../prediction/prediction.hpp"

void Aimbot::Init()
{

}

void Aimbot::Invoke()
{
	lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());

	if (lp->IsAlive())
	{
		lpeyepos = lp->GetEyeOffset() + lp->GetAbsOrigin();
		cmd = GetArg<CUserCmd*>(GetArguments(CREATEMOVE), 0);
		before = cmd->viewangles;
		weapon = lp->GetWeapon();

		rage.Invoke();
	}
}

static Vector empty(0, 0, 0);
Vector Aimbot::GetHitbox(C_BaseEntity* p, int index)
{
	static const model_t* model;
	static studiohdr_t* hdr;
	static VMatrix bones[128];
	if (lastplayer != p)
	{
		lastplayer = p;

		model = p->GetModel();
		if (!model)
			return empty;

		hdr = modelinfo->GetStudiomodel(model);
		if (!hdr)
			return empty;

		bones[128];
		if (!p->SetupBones(bones, globals->curtime))
			return empty;
	}

	static mstudiobbox_t* hitbox = hdr->GetHitbox(index, 0);
	if (!hitbox)
		return empty;

	float mod = hitbox->m_flRadius != -1.f ? hitbox->m_flRadius : 0.f;

	Vector min, max;
	VectorTransform(hitbox->bbmin - mod, bones[hitbox->bone], min);
	VectorTransform(hitbox->bbmax + mod, bones[hitbox->bone], max);

	return (min + max) * 0.5f;
}

void Aimbot::CalculateAngle(const Vector& pos, Angle& out)
{
	VectorAngles(pos - lpeyepos, out);
}

bool Aimbot::IsVisible(C_BaseEntity* p, const Vector& pos)
{
	static CTraceFilterDouble filter;
	static trace_t tr;
	static Ray_t ray;

	ray.Init(lpeyepos, pos);
	filter.pSkip1 = lp;
	filter.pSkip2 = p;
	enginetrace->TraceRay(ray, 0x46004003, &filter, &tr);

	return (tr.m_pEnt == p || tr.fraction == 1);
}

void Aimbot::MovementFix()
{
	Vector move(cmd->move.x, cmd->move.y, 0);
	float speed = move.Length2D();

	Angle view = cmd->viewangles;
	view.p = clamp(view.p, -89.f, 89.f);
	view.y = clamp(normalize(view.y), -180.f, 180.f);

	float yaw = Rad2Deg(atan2(move.y, move.x));
	yaw = Deg2Rad(view.y - before.y + yaw);

	cmd->move.x = cos(yaw) * speed;
	cmd->move.y = sin(yaw) * speed;
}

bool Aimbot::CanShoot()
{
	if (!weapon)
		return false;

	if (lp->GetNextPrimaryAttack(weapon) > predict.pred_time)
		return false;

	return true;
}

void Aimbot::NoRecoil()
{
	cmd->viewangles -= (lp->GetAimPunch() * 2);
	cmd->viewangles.r = 0.f;
}

void Aimbot::End()
{
	lp = nullptr;
	lastplayer = nullptr;
	lpeyepos = empty;
	before = Angle(0, 0, 0);
	weapon = nullptr;
}

void Aimbot::Destroy()
{

}

Aimbot aimbot;

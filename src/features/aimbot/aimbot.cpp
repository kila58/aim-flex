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

	Clamp();
}

static Vector empty(0, 0, 0);
Vector Aimbot::GetHitbox(C_BaseEntity* p, int index)
{
	const model_t* model = p->GetModel();
	if (!model)
		return empty;

	studiohdr_t* hdr = modelinfo->GetStudiomodel(model);
	if (!hdr)
		return empty;

	VMatrix bones[128];
	if (!p->SetupBones(bones, globals->curtime))
		return empty;

	mstudiobbox_t* hitbox = hdr->GetHitbox(index, 0);
	if (!hitbox || hitbox->bone > 128 || hitbox->bone < 0 || hitbox->group > 7)
		return empty;

	float mod = hitbox->m_flRadius != -1.f ? hitbox->m_flRadius : 0.f;

	Vector min, max;
	VectorTransform(hitbox->bbmin - mod, bones[hitbox->bone], min);
	VectorTransform(hitbox->bbmax + mod, bones[hitbox->bone], max);

	return (min + max) * 0.5f;
}

Vector Aimbot::GetBodyAim(C_BaseEntity* p)
{
	Vector mins, maxs;
	p->GetRenderable()->GetRenderBounds(mins, maxs);

	Vector origin = p->GetAbsOrigin();
	origin.z += ((mins.z + maxs.z) * 0.5f);

	return origin;
}

bool Aimbot::GetHitboxes(C_BaseEntity* p, Hitboxes& hitboxes)
{
	static const model_t* model;
	static studiohdr_t* hdr;
	if (lastplayer2 != p)
	{
		lastplayer2 = p;

		model = p->GetModel();
		if (!model)
			return false;

		hdr = modelinfo->GetStudiomodel(model);
		if (!hdr)
			return false;
	}

	for (int h = 0; h < hdr->GetHitboxCount(0); h++)
	{
		Vector pos = GetHitbox(p, h);

		if (!pos.IsZero())
			hitboxes.emplace_back(pos, h);
	}

	if (!hitboxes.empty())
	{
		Vector& top = hitboxes.front().center;

		std::sort(hitboxes.begin(), hitboxes.end(), [&top](const Hitbox& a, const Hitbox& b)
		{
			return a.center.Distance(top) < b.center.Distance(top);
		});

		return true;
	}

	return false;
}

void Aimbot::CalculateAngle(const Vector& pos, Angle& out)
{
	VectorAngles(pos - lpeyepos, out);
}

bool Aimbot::IsVisible(C_BaseEntity* p, const Vector& pos)
{
	static CTraceFilterDouble filter;
	static trace_t trace;
	static Ray_t ray;

	ray.Init(lpeyepos, pos);
	filter.pSkip1 = lp;
	filter.pSkip2 = p;
	enginetrace->TraceRay(ray, 0x46004003, &filter, &trace);

	return (trace.m_pEnt == p || trace.fraction == 1);
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

	if (!(weapon->GetAmmo() > 0))
		return false;

	Activity act = weapon->GetActivity();
	if (act == ACT_RESET || act == ACT_VM_DRAW || act == ACT_VM_RELOAD)
		return false;

	if (lp->GetNextPrimaryAttack(weapon) > predict.pred_time)
		return false;

	return true;
}

void Aimbot::NoRecoil()
{
	cmd->viewangles -= (lp->GetAimPunch() * 2);
}

void Aimbot::Clamp()
{
	if (cmd)
	{
		cmd->viewangles.p = clamp(cmd->viewangles.p, -89.f, 89.f);
		cmd->viewangles.y = clamp(normalize(cmd->viewangles.y), -180.f, 180.f);
		cmd->viewangles.r = 0.f;
	}
}

void Aimbot::End()
{
	lp = nullptr;
	lastplayer = nullptr;
	lastplayer2 = nullptr;
	lpeyepos = empty;
	before = Angle(0, 0, 0);
	weapon = nullptr;
}

void Aimbot::Destroy()
{

}

Aimbot aimbot;

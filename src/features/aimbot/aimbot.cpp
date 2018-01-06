#include "aimbot.hpp"

#include "../../aim-flex.hpp"

#include "../rage/rage.hpp"
#include "../legit/legit.hpp"
#include "../prediction/prediction.hpp"
#include "../playermanager/playermanager.hpp"

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

bool Aimbot::SetupBones(C_BaseEntity* p, int bonemask, VMatrix* bones)
{
	IClientRenderable* renderable = p->GetRenderable();
	if (!renderable)
		return false;

	CStudioHdr* hdr = p->GetModelPtr();
	if (!hdr)
		return false;

	VMatrix* backup_matrix = renderable->GetBoneArrayForWrite();
	if (!backup_matrix)
		return false;

	auto& player = playermanager.GetPlayer(p);
	if (!player)
		return false;

	auto old = p->GetPVSFlag();
	p->SetPVSFlag(0);

	Vector origin = p->GetOrigin();
	Angle angles = player.resolverinfo.absang;

	Vector backup_absorigin = p->GetAbsOrigin();
	Angle backup_absangles = p->GetAbsAngles();

	float backup_poses[24];
	p->GetPoseParameters(backup_poses);

	auto backup_layers = p->GetAnimLayers();

	VMatrix cameratransform;
	AngleMatrix(backup_absangles, origin, cameratransform);

	float* poses_uninterp = player.poses;
	auto layers_uninterp = player.animationlayers;

	p->SetAbsOrigin(origin);
	p->SetAbsAngles(angles);
	p->SetPoseParameters(poses_uninterp);
	p->SetAnimLayers(layers_uninterp);

	Vector pos[128];
	Quaternion q[128];

	p->StandardBlendingRules(hdr, pos, q, predict.pred_time, bonemask | 0x80000);

	renderable->SetBoneArrayForWrite(bones);

	byte computed[0x100];
	p->BuildTransformations(hdr, pos, q, cameratransform, bonemask | 0x80000, computed);

	renderable->SetBoneArrayForWrite(backup_matrix);

	p->SetAbsOrigin(backup_absorigin);
	p->SetAbsAngles(backup_absangles);
	p->SetPoseParameters(backup_poses);
	p->SetAnimLayers(backup_layers);

	p->SetPVSFlag(old);

	return true;
}

static Vector empty(0, 0, 0);
Vector Aimbot::GetHitbox(C_BaseEntity* p, int index, bool interpolated)
{
	static const model_t* model;
	static studiohdr_t* hdr;
	static VMatrix bones_interp[128];
	static VMatrix bones_uninterp[128];
	static bool setupbones_interp = false;
	static bool setupbones_uninterp = false;
	if (lastplayer != p)
	{
		model = p->GetModel();
		if (!model)
			return empty;

		hdr = p->GetModelPtr()->studio;
		if (!hdr)
			return empty;

		setupbones_interp = false;
		setupbones_uninterp = false;
	}

	if (interpolated && !setupbones_interp)
	{
		if (!p->SetupBones(bones_interp, globals->curtime))
			return empty;

		setupbones_interp = true;
	}
	else if (!setupbones_uninterp)
	{
		if (!SetupBones(p, BONE_USED_BY_HITBOX, bones_uninterp))
			return empty;

		setupbones_uninterp = true;
	}

	mstudiobbox_t* hitbox = hdr->GetHitbox(index, 0);
	if (!hitbox || hitbox->bone > 128 || hitbox->bone < 0 || hitbox->group > 7)
		return empty;

	float mod = hitbox->m_flRadius != -1.f ? hitbox->m_flRadius : 0.f;

	Vector min, max;

	if (interpolated)
	{
		VectorTransform(hitbox->bbmin - mod, bones_interp[hitbox->bone], min);
		VectorTransform(hitbox->bbmax + mod, bones_interp[hitbox->bone], max);
	}
	else
	{
		VectorTransform(hitbox->bbmin - mod, bones_uninterp[hitbox->bone], min);
		VectorTransform(hitbox->bbmax + mod, bones_uninterp[hitbox->bone], max);
	}

	lastplayer = p;

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

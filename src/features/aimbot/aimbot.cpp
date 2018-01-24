#include "aimbot.hpp"

#include "../../aim-flex.hpp"

#include "../rage/rage.hpp"
#include "../legit/legit.hpp"
#include "../prediction/prediction.hpp"
#include "../playermanager/playermanager.hpp"
#include "../settings/settings.hpp"

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
	if (!player || !player.dormantplayer->animstate)
		return false;

	Vector origin = p->GetOrigin();
	Angle angles = player.resolverinfo.absang;

	Vector backup_absorigin = p->GetAbsOrigin();
	Angle backup_absangles = p->GetAbsAngles();

	auto backup_poses = p->GetPoseParameters();
	auto backup_layers = p->GetAnimLayers();

	alignas(16) VMatrix cameratransform;
	AngleMatrix(backup_absangles, origin, cameratransform);

	auto poses_uninterp = player.poses;
	auto layers_uninterp = player.animationlayers;

	p->SetAbsOrigin(origin);
	p->SetAbsAngles(angles);
	p->SetPoseParameters(poses_uninterp);
	p->SetAnimLayers(layers_uninterp);

	Vector pos[128];
	Quaternion q[128];

	p->StandardBlendingRules(hdr, pos, q, globals->curtime, bonemask | 0x80000);

	renderable->SetBoneArrayForWrite(bones);

	byte computed[0x100] = {};
	p->BuildTransformations(hdr, pos, q, cameratransform, bonemask | 0x80000, computed);

	renderable->SetBoneArrayForWrite(backup_matrix);

	p->SetAbsOrigin(backup_absorigin);
	p->SetAbsAngles(backup_absangles);
	p->SetPoseParameters(backup_poses);
	p->SetAnimLayers(backup_layers);

	return true;
}

bool Aimbot::HitChance(C_BaseEntity* target, const Angle& ang)
{
	Vector forward, right, up;
	AngleVectors(ang, forward, right, up);

	int max = 256;
	int hits = 0;
	int min = (int)(max * (settings.Get<float>("rage_hitchance_value") / 100.f));

	float range = weapon->GetCSWpnData()->m_flRange;

	lp->GetWeapon()->UpdateAccuracyPenalty();

	for (int i = 0; i < max; i++)
	{
		RandomSeed(i + 1);

		float inaccuracy = RandomFloat(0.f, 1.f) * weapon->GetInaccuracy();
		float spread = RandomFloat(0.f, 1.f) * weapon->GetSpread();

		float pi1 = RandomFloat(0.f, 2.f * pi);
		float pi2 = RandomFloat(0.f, 2.f * pi);

		float spreadx = cos(pi1) * inaccuracy + cos(pi1) * spread;
		float spready = sin(pi2) * inaccuracy + sin(pi2) * spread;

		Vector dir = forward + (right * -spreadx) + (up * -spready);

		if (DoesIntersectCapsule(lpeyepos, dir, tick->hitboxinfo.mins, tick->hitboxinfo.maxs, tick->hitboxinfo.radius, range))
			hits++;

		if (hits >= min)
			return true;

		if (((max - i) + hits) < min)
			break;
	}

	return false;
}

static Vector empty(0, 0, 0);
Vector Aimbot::GetHitbox(C_BaseEntity* p, int index, bool interpolated)
{
	static const model_t* model;
	static studiohdr_t* hdr;
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

	hitbox = hdr->GetHitbox(index, 0);
	if (!hitbox || hitbox->bone > 128 || hitbox->bone < 0 || hitbox->group > 7)
		return empty;

	float mod = hitbox->m_flRadius != -1.f ? hitbox->m_flRadius : 0.f;

	if (interpolated)
	{
		VectorTransform(hitbox->bbmin - mod, bones_interp[hitbox->bone], mins);
		VectorTransform(hitbox->bbmax + mod, bones_interp[hitbox->bone], maxs);

		VectorTransform(hitbox->bbmin, bones_interp[hitbox->bone], minsnoradius);
		VectorTransform(hitbox->bbmax, bones_interp[hitbox->bone], maxsnoradius);
	}
	else
	{
		VectorTransform(hitbox->bbmin - mod, bones_uninterp[hitbox->bone], mins);
		VectorTransform(hitbox->bbmax + mod, bones_uninterp[hitbox->bone], maxs);

		VectorTransform(hitbox->bbmin, bones_uninterp[hitbox->bone], minsnoradius);
		VectorTransform(hitbox->bbmax, bones_uninterp[hitbox->bone], maxsnoradius);
	}

	lastplayer = p;

	return (mins + maxs) * 0.5f;
}

void Aimbot::GetMatrix(bool interpolated, VMatrix* matrix)
{
	if (!hitbox)
		return;

	matrix = interpolated ? &bones_interp[hitbox->bone] : &bones_uninterp[hitbox->bone];
}

void Aimbot::GetHitboxBounds(Vector& mins, Vector& maxs)
{
	mins = this->mins;
	maxs = this->maxs;
}

void Aimbot::GetHitboxBoundsNoRadius(Vector& mins, Vector& maxs)
{
	mins = this->minsnoradius;
	maxs = this->maxsnoradius;
}

float Aimbot::GetRadius()
{
	return hitbox->m_flRadius;
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

bool Aimbot::MultiPoint(C_BaseEntity* p, int index, Vector& out)
{
	Vector min = tick->hitboxinfo.minsnoradius, max = tick->hitboxinfo.maxsnoradius;

	Vector delta = max - min;
	VectorNormalize(delta);

	float radius = tick->hitboxinfo.radius;

	std::deque<Vector> spheres;

	spheres.push_back(min);

	for (int i = 1; i < std::floor(min.Distance(max)); i++)
		spheres.push_back(min + delta * i);

	spheres.push_back(max);

	float height = radius * 2;

	for (auto& s : spheres)
	{
		for (float z = 0; z <= height; z++)
		{
			float radius_mod = radius;

			bool bottom = z < radius;
			bool top = z > radius;

			if (bottom || top)
			{
				if (!(&spheres.front() == &s || &spheres.back() == &s) && !(z == 0 || z == height))
					continue;

				float distance = bottom ? z - radius : z - (height - radius); // distance from center according to radius
				radius_mod = sqrt((radius * radius) - (distance * distance)); // calculate chord radius modifier
			}

			for (float y = 0; y < 360; y += 10)
			{
				Vector forward, right, up;
				AngleVectors(Angle(0, y, 0), forward, right, up);

				Vector final = Vector(s.x, s.y, (s.z - radius) + z) + (forward * radius_mod);

				if (aimbot.IsVisible(p, final))
				{
					out = final;

					return true;
				}
			}
		}
	}

	return false;
}

void Aimbot::CalculateAngle(const Vector& pos, Angle& out)
{
	VectorAngles(pos - lpeyepos, out);
}

bool Aimbot::IsVisible(C_BaseEntity* p, const Vector& pos, int tracetype, bool checkfraction)
{
	static CTraceFilterDouble filter;
	static trace_t trace;
	static Ray_t ray;

	ray.Init(lpeyepos, pos);
	filter.pSkip1 = lp;

	if (checkfraction)
		filter.pSkip2 = p;

	filter.type = TraceType_t(tracetype);
	enginetrace->TraceRay(ray, 0x46004003, &filter, &trace);

	bool ret = trace.m_pEnt == p;
	if (checkfraction && trace.fraction == 1.f)
		ret = true;

	return ret;
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

	if (lp->GetNextPrimaryAttack(weapon) > globals->curtime)
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
	hitbox = nullptr;
	target = nullptr;
	tick = nullptr;
}

void Aimbot::Destroy()
{

}

Aimbot aimbot;

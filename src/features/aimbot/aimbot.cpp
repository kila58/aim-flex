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

// need to feed traces new setupbones info as well...
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
	AngleMatrix(angles, origin, cameratransform);

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
		Vector end = lpeyepos + (dir * range);

		if (DoesIntersectCapsule(lpeyepos, dir, tick->hitboxinfo.minsnoradius, tick->hitboxinfo.maxsnoradius, tick->hitboxinfo.radius, range))
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

	hitboxindex = index;

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
	return hitbox ? hitbox->m_flRadius : 0.f;
}

int Aimbot::GetHitboxIndex()
{
	return hitboxindex;
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

static Tick invalidtick;
bool Aimbot::MultiPoint(C_BaseEntity* p, Tick& tick, Vector& out)
{
	Vector min, max;
	if (tick)
		min = tick.hitboxinfo.minsnoradius, max = tick.hitboxinfo.maxsnoradius;
	else
		aimbot.GetHitboxBoundsNoRadius(min, max);

	int h;
	if (tick)
		h = tick.hitboxinfo.hitbox;
	else
		h = aimbot.GetHitboxIndex();

	Vector delta = max - min;
	VectorNormalize(delta);

	float radius = tick ? tick.hitboxinfo.radius : aimbot.GetRadius();

	// add scaling to a setting
	radius = radius * 0.8f;

	std::deque<Vector> spheres;

	spheres.push_back(min);

	for (int i = 1; i < std::floor(min.Distance(max)); i++)
		spheres.push_back(min + delta * i);

	spheres.push_back(max);

	float height = radius * 2;

	std::deque<Vector> positions;

	for (auto it = spheres.begin(); it != spheres.end(); ++it)
	{
		auto& s = *it;

		for (float z = 0; z <= height; z += 2)
		{
			float radius_mod = radius;

			bool bottom = z < radius;
			bool top = z > radius;

			if (bottom || top)
			{
				if (h == HITBOX_LEFTTRICEP || h == HITBOX_RIGHTTRICEP
					|| (h == HITBOX_LEFTLEG || h == HITBOX_RIGHTLEG)
					|| (h == HITBOX_LEFTFOREARM || h == HITBOX_RIGHTFOREARM))
				{
					auto nice = std::distance(spheres.begin(), it);
					if (nice % 2)
						continue;
				}
				else if (!(&spheres.front() == &s || &spheres.back() == &s) && !(z == 0 || z == height))
					continue;

				float distance = bottom ? z - radius : z - (height - radius); // distance from center according to radius
				radius_mod = sqrt((radius * radius) - (distance * distance)); // calculate chord radius modifier
			}

			Angle projection;
			VectorAngles(s - lpeyepos, projection);

			for (float y = projection.y - 90.f; y < projection.y + 90; y += 20)
			{
				Vector forward, right, up;
				AngleVectors(Angle(0, y, 0), forward, right, up);

				Vector final = Vector(s.x, s.y, (s.z - radius) + z) + (forward * radius_mod);
				positions.emplace_back(final);
			}
		}
	}

	// right now we are sorting by the highest, soon add feature so you can choose spot from argument in a bitwise flag
	// also make it dynamic based on their pitch
	// add a fraction check
	Vector origin = p->GetAbsOrigin();
	origin.z += 128.f;

	std::sort(positions.begin(), positions.end(), [&origin](const Vector& a, const Vector& b)
	{
		return a.Distance(origin) < b.Distance(origin);
	});

	for (auto& pos : positions)
	{
		if (aimbot.IsVisible(p, pos))
		{
			out = pos;
			
			return true;
		}
	}

	return false;
}

bool Aimbot::MultiPoint(C_BaseEntity* p, Vector& out)
{
	return MultiPoint(p, invalidtick, out);
}

bool Aimbot::MultiPoint(C_BaseEntity* p, int index, bool interpolated, Vector& out)
{
	// update min max and radius cache
	aimbot.GetHitbox(p, index, interpolated);

	return MultiPoint(p, out);
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
	view.p = std::clamp(view.p, -89.f, 89.f);
	view.y = std::clamp(normalize(view.y), -180.f, 180.f);

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
		cmd->viewangles.p = std::clamp(cmd->viewangles.p, -89.f, 89.f);
		cmd->viewangles.y = std::clamp(normalize(cmd->viewangles.y), -180.f, 180.f);
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
	//target = nullptr;
	tick = nullptr;
}

void Aimbot::Destroy()
{

}

Aimbot aimbot;

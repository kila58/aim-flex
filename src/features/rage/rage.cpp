#include "rage.hpp"

#include "../../aim-flex.hpp"

#include "../playermanager/playermanager.hpp"
#include "../aimbot/aimbot.hpp"

void Rage::Init()
{

}

bool MultiBox(C_BaseEntity* p, Vector& hitbox)
{
	if (!aimbot.IsVisible(p, hitbox))
	{
		model_t* model = p->GetModel();
		if (!model)
			return false;

		studiohdr_t* hdr = modelinfo->GetStudiomodel(model);
		if (!hdr)
			return false;

		std::deque<Vector> hitboxes;

		for (int i = 0; i <= hdr->GetHitboxCount(0); i++)
		{
			Vector pos = aimbot.GetHitbox(p, i);
			if (pos.IsZero())
				continue;

			if (!aimbot.IsVisible(p, pos))
				continue;

			hitboxes.emplace_back(pos);
		}

		std::sort(hitboxes.begin(), hitboxes.end(), [hitbox](const Vector& a, const Vector& b)
		{
			return a.Distance(hitbox) < b.Distance(hitbox);
		});

		if (!hitboxes.empty())
		{
			hitbox = hitboxes.front();

			return true;
		}

		return false;
	}
}

bool FindTarget(Angle& ang)
{
	C_BaseEntity* lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());
	auto& targets = playermanager.GetPlayers();

	for (auto& target : targets)
	{
		C_BaseEntity* p = target.ent;
		Vector& center = aimbot.GetHitbox(p, 0);

		MultiBox(p, center);

		if (!center.IsZero() && aimbot.IsVisible(p, center))
		{
			aimbot.CalculateAngle(center, ang);

			ang -= (lp->GetAimPunch() * 2);
			ang.r = 0.f;

			return true;
		}
	}

	return false;
}

void Rage::Invoke()
{
	auto cmd = GetArg<CUserCmd*>(GetArguments(CREATEMOVE), 0);

	//cvar->ConsoleColorPrintf(std::to_string(aimbot.CanShoot()) + "\n");

	Angle ang;
	if (aimbot.CanShoot() && FindTarget(ang))
	{
		cmd->viewangles = ang;
		cmd->buttons |= IN_ATTACK;
	}
}

void Rage::Destroy()
{

}

Rage rage;

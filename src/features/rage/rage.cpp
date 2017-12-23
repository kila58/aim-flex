#include "rage.hpp"

#include "../../aim-flex.hpp"

#include "../playermanager/playermanager.hpp"
#include "../aimbot/aimbot.hpp"
#include "../prediction/prediction.hpp"

void Rage::Init()
{

}

bool FindTarget(CUserCmd* cmd, Angle& ang)
{
	for (auto& target : playermanager.GetPlayers())
	{
		C_BaseEntity* p = target.ent;

		/*
		const model_t* model = p->GetModel();
		if (!model)
			continue;

		studiohdr_t* hdr = modelinfo->GetStudiomodel(model);
		if (!hdr)
			continue;

		VMatrix bones[128];
		if (!p->SetupBones(bones, globals->curtime))
			continue;

		mstudiobbox_t* hitbox = hdr->GetHitbox(3, 0);
		if (!hitbox)
			continue;

		float mod = hitbox->m_flRadius != -1.f ? hitbox->m_flRadius : 0.f;

		Vector min, max;
		VectorTransform(hitbox->bbmin - mod, bones[hitbox->bone], min);
		VectorTransform(hitbox->bbmax + mod, bones[hitbox->bone], max);

		Vector pos = (min + max) * 0.5f;

		static CTraceFilterDouble filter;
		static trace_t tr;
		static Ray_t ray;

		if (aimbot.IsVisible(p, pos))
		{
			aimbot.CalculateAngle(pos, ang);

			return true;
		}
		*/

		/*
		Hitboxes hitboxes;
		if (aimbot.GetHitboxes(p, hitboxes))
		{
			for (auto& hitbox : hitboxes)
			{
				Vector& pos = hitbox.center;

				if (aimbot.IsVisible(p, pos))
				{
					aimbot.CalculateAngle(pos, ang);

					return true;
				}
			}
		}
		*/

		/*
		if (target.backtrackinfo.ticks.size() > 1)
		{
			auto& first = target.backtrackinfo.ticks.back();
			auto& last = target.backtrackinfo.ticks.front();

			if (!first.head.IsZero() && !last.head.IsZero())
			{
				if (!aimbot.IsVisible(p, first.head) && aimbot.IsVisible(p, last.head))
				{
					aimbot.CalculateAngle(last.head, ang);
					backtrack.BacktrackToTick(cmd, last);

					return true;
				}
			}
		}
		*/

		Vector pos = aimbot.GetHitbox(p, 3);
		
		if (!pos.IsZero()/* && (aimbot.IsVisible(p, pos))*/)
		{
			aimbot.CalculateAngle(pos, ang);

			return true;
		}
	}

	return false;
}

void Rage::Invoke()
{
	C_BaseEntity* lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());
	auto cmd = GetArg<CUserCmd*>(GetArguments(CREATEMOVE), 0);

	Angle ang;
	if (lp->IsAlive() && aimbot.CanShoot() && FindTarget(cmd, ang))
	{
		cmd->viewangles = ang;
		aimbot.NoRecoil();

		cmd->buttons |= IN_ATTACK;

		//aimbot.MovementFix();
	}
}

void Rage::Destroy()
{

}

Rage rage;

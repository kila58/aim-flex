#include "rage.hpp"

#include "../../aim-flex.hpp"

#include "../playermanager/playermanager.hpp"
#include "../aimbot/aimbot.hpp"
#include "../prediction/prediction.hpp"
#include "../antiaim/antiaim.hpp"

void Rage::Init()
{

}

bool Rage::FindTarget(CUserCmd* cmd, Angle& ang)
{
	for (auto& target : playermanager.GetPlayers())
	{
		C_BaseEntity* p = target.ent;

		/*for (auto& tick : target.backtrackinfo.ticks)
		{
			if (aimbot.IsVisible(p, tick.hitboxinfo.head))
			{
				aimbot.CalculateAngle(tick.hitboxinfo.head, ang);

				aimbot.target = p;
				aimbot.tick = &tick;

				return true;
			}
		}*/

		/*
		Vector head = aimbot.GetHitbox(p, 0);
		if (!head.IsZero() && aimbot.IsVisible(p, head))
		{
			aimbot.CalculateAngle(head, ang);
		
			return true;
		}
		*/
		
		/*
		Vector head = aimbot.GetHitbox(p, 0);
		if (!head.IsZero() && !aimbot.IsVisible(p, head) && target.backtrackinfo.ticks.size() > 1)
		{
			auto& last = target.backtrackinfo.ticks.front();

			for (auto& tick : target.backtrackinfo.ticks)
			{
				if (tick == last && aimbot.IsVisible(p, tick.head))
				{
					aimbot.CalculateAngle(last.head, ang);

					aimbot.target = p;
					aimbot.tick = &tick;

					return true;
				}

				if (aimbot.IsVisible(p, tick.head))
					break;
			}
		}
		*/

		/*
		auto& sim = target.backtrackinfo.FindTick(p, p->GetSimulationTime());
		//cvar->ConsoleColorPrintf("sim:" + std::to_string(p->GetSimulationTime()) + "\n");
		auto& first = target.backtrackinfo.ticks.back();
		auto& last = target.backtrackinfo.ticks.front();
		if (first && last && sim && aimbot.IsVisible(p, sim.head))
		{	
			if (TIME_TO_TICKS(first.time - last.time) == 15)
			{
				cvar->ConsoleColorPrintf(std::to_string(TIME_TO_TICKS(first.time - last.time)) + "\n");

				aimbot.CalculateAngle(sim.head, ang);
				backtrack.BacktrackToTick(cmd, sim);

				return true;
			}
		}
		*/

		//cmd->buttons &= ~IN_ATTACK;

		//for (auto& tick : target.backtrackinfo.ticks)
		//if (target.backtrackinfo.ticks.size() > 2)
		//{
		//	auto& last = target.backtrackinfo.FindTick(p, p->GetSimulationTime());
			//auto& last = target.backtrackinfo.ticks.front();
		//	auto& first = target.backtrackinfo.ticks.back();
		//	if (last && first)
		//	{
		//		if (aimbot.IsVisible(p, last.head) && !aimbot.IsVisible(p, first.head))
		//		{
		//			aimbot.CalculateAngle(last.head, ang);
		//			backtrack.BacktrackToTick(cmd, last);
		//
		//			return true;
		//		}
				//}
		//	}
		//}

		Hitboxes hitboxes;
		if (aimbot.GetHitboxes(p, hitboxes))
		{
			for (auto& hitbox : hitboxes)
			{
				Vector& pos = hitbox.center;

				if (!aimbot.IsVisible(p, pos))
				{
					if (aimbot.MultiPoint(p, hitbox.index, pos))
					{
						aimbot.CalculateAngle(pos, ang);

						return true;
					}
				}
				else
				{
					aimbot.CalculateAngle(pos, ang);

					return true;
				}
			}
		}
	}

	return false;
}

void Rage::Invoke()
{
	C_BaseEntity* lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());
	auto cmd = GetArg<CUserCmd*>(GetArguments(CREATEMOVE), 0);

	Angle ang;
	if (aimbot.CanShoot() && FindTarget(cmd, ang))
	{
		//if (false)
		if (aimbot.HitChance(aimbot.target, ang))
		{
			backtrack.BacktrackToTick(cmd, *aimbot.tick);

			cmd->viewangles = ang;
			aimbot.NoRecoil();

			if (aimbot.CanShoot())
				cmd->buttons |= IN_ATTACK;

			aimbot.MovementFix();
		}
	}
	else if (aimbot.CanShoot() && cmd->buttons & IN_ATTACK)
	{
		aimbot.NoRecoil();
	}
	else
	{
		antiaim.Invoke();

		aimbot.MovementFix();
	}
}

void Rage::Destroy()
{

}

Rage rage;

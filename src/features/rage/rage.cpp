#include "rage.hpp"

#include "../../aim-flex.hpp"

#include "../playermanager/playermanager.hpp"
#include "../aimbot/aimbot.hpp"
#include "../antiaim/antiaim.hpp"
#include "../settings/settings.hpp"

void Rage::Init()
{

}

bool Rage::FindTarget(CUserCmd* cmd, Angle& ang)
{
	for (auto& target : playermanager.GetPlayers())
	{
		C_BaseEntity* p = target.ent;

		// troll aim won't work right now, because of vischeck????(?????) in multipoint (add arg)?
		/*
		if (target.backtrackinfo.ticks.size() > 1)
		{
			auto& last = target.backtrackinfo.ticks.front();

			for (auto& tick : target.backtrackinfo.ticks)
			{
				Vector out;
				if (aimbot.MultiPoint(p, tick, out))
				{
					if (tick == last)
					{
						if (aimbot.IsVisible(p, out))
						{
							aimbot.CalculateAngle(out, ang);

							aimbot.target = p;
							aimbot.tick = &tick;

							return true;
						}
					}

					if (aimbot.IsVisible(p, out))
						return false;
				}
			}
		}
		*/

		for (auto& tick : target.backtrackinfo.ticks)
		{
			Vector out = tick.hitboxinfo.head;
			
			if (settings.Get<bool>("rage_multipoint_enabled"))
				aimbot.MultiPoint(p, tick, out);

			if (aimbot.IsVisible(p, out))
			{
				aimbot.CalculateAngle(out, ang);

				aimbot.target = p;
				aimbot.tick = &tick;

				return true;
			}
		}

		/*
		//for (auto& tick : target.backtrackinfo.ticks)
		if (false)
		{
			Vector out;

			//benchmark.Start("Rage::FindTarget");
			bool cont = aimbot.MultiPoint(p, 0, true, out);
			//benchmark.End("Rage::FindTarget");

			if (cont)
			{
				//if (aimbot.IsVisible(p, out))
				{
					aimbot.CalculateAngle(out, ang);

					aimbot.target = p;
					//aimbot.tick = &tick;

					return true;
				}
			}
		}
		*/

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

		/*
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
		*/
	}

	return false;
}

void Rage::Invoke()
{
	C_BaseEntity* lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());
	auto cmd = GetArg<CUserCmd*>(GetArguments(CREATEMOVE), 0);
	auto frametime = GetArg<float>(GetArguments(CREATEMOVE), 2);

	Angle ang;
	if (lp && aimbot.CanShoot() && FindTarget(cmd, ang))
	{
		bool hitchance_pass = true;

		if (settings.Get<bool>("rage_hitchance_enabled"))
			hitchance_pass = aimbot.HitChance(aimbot.target, ang);

		if (hitchance_pass)
		{
			if (aimbot.tick)
				backtrack.BacktrackToTick(cmd, *aimbot.tick);

			cmd->viewangles = ang;
			if (!settings.Get<bool>("rage_silent_enabled"))
				engineclient->SetViewAngles(ang);

			aimbot.NoRecoil();

			cmd->buttons |= IN_ATTACK;

			aimbot.MovementFix();

			*aimbot.bsendpacket = false;
		}
	}
	else if (aimbot.CanShoot() && cmd->buttons & IN_ATTACK)
	{
		aimbot.NoRecoil();

		*aimbot.bsendpacket = false;
	}
	else
	{
		antiaim.Invoke();

		aimbot.MovementFix();
	}

	BaseFeature::SetArguments(CREATEMOVE, cmd, false, frametime);
}

void Rage::Destroy()
{

}

Rage rage;

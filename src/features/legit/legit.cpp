#include "legit.hpp"

#include "../../aim-flex.hpp"

#include "../playermanager/playermanager.hpp"
#include "../aimbot/aimbot.hpp"
#include "../settings/settings.hpp"
#include "../input/input.hpp"

void Legit::Init()
{
	ResetTime();
}

bool Legit::FindTarget(CUserCmd* cmd, Angle& ang)
{
	C_BaseEntity* lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());
	if (!lp)
		return false;

	Vector lpeyepos = lp->GetEyePos();

	auto weapon = lp->GetWeapon();
	if (!weapon)
		return false;

	//auto type = weapon->GetCSWpnData()-> m_iItemDefinitionIndex

	float fov = (float)settings.Get<int>("legit_fov");

	for (auto& target : playermanager.GetPlayers())
	{
		C_BaseEntity* p = target.ent;

		/*
		Vector pos_diff = ((p->GetEyeOffset() + p->GetAbsOrigin()) - lpeyepos);
		Angle current_angle;
		VectorAngles(pos_diff, current_angle);

		float diff = CalculateFOV(cmd->viewangles, current_angle);
		*/

		//debug << "diff: " << diff << "\n";
		//debug << "fov: " << fov << "\n";

		//if (diff < fov || fov == 0)
		{
			Hitboxes hitboxes;
			bool hitboxes_found = false;

			if (weapon->GetItemDefinitionIndex() == WEAPON_AWP)
			{
				hitboxes_found = aimbot.GetHitboxes(p, hitboxes, { HITBOX_MIDDLEBODY, HITBOX_HEAD });
			}
			else
			{
				hitboxes_found = aimbot.GetHitboxes(p, hitboxes, { HITBOX_HEAD });
			}

			if (hitboxes_found)
			{
				for (auto& hitbox : hitboxes)
				{
					Vector& out = hitbox.center;

					aimbot.CalculateAngle(out, ang);
					float diff = CalculateFOV(cmd->viewangles, ang - (lp->GetAimPunch() * 2));

					if (diff < fov || fov == 0)
					{
						if (aimbot.IsVisible(p, out))
						{
							aimbot.target = p;

							return true;
						}
					}
				}
			}
		}

		/*
		for (auto& tick : Reverse(target.backtrackinfo.ticks))
		{
			Vector out = tick.hitboxinfo.head;

			if (aimbot.IsVisible(p, out))
			{
				aimbot.CalculateAngle(out, ang);

				float fov = (float)settings.Get<int>("legit_fov");

				Angle current_angle = cmd->viewangles;
				current_angle -= (lp->GetAimPunch() * 2);

				float diff = abs(normalize(ang.p - current_angle.p)) + abs(normalize(ang.y - current_angle.y));

				if (diff < fov || fov == 0)
				{
					aimbot.target = p;
					aimbot.tick = &tick;

					return true;
				}
			}
		}
		*/
	}

	return false;
}

void Legit::ResetTime()
{
	smooth_time = 0.f;//(float)(settings.Get<int>("legit_speed_start")) / 100.f;
}

void Legit::Invoke()
{
	auto cmd = GetArg<CUserCmd*>(GetArguments(CREATEMOVE), 0);
	auto frametime = GetArg<float>(GetArguments(CREATEMOVE), 2);
	
	C_BaseEntity* lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());
	if (!lp)
	{
		BaseFeature::SetArguments(CREATEMOVE, cmd, true, frametime);

		return;
	}

	auto weapon = lp->GetWeapon();
	if (!weapon)
	{
		BaseFeature::SetArguments(CREATEMOVE, cmd, true, frametime);

		return;
	}

	if (input.KeyDown(settings.Get<int>("legit_aimkey")))
	{
		Angle ang;
		if (FindTarget(cmd, ang))
		{
			int shots_fired = lp->GetShotsFired();

			if (last_target != aimbot.target)
				ResetTime();

			if (last_shotsfired > 0 && !(cmd->buttons & IN_ATTACK))
				ResetTime();

			Angle real_ang = cmd->viewangles;

			if (aimbot.tick)
				backtrack.BacktrackToTick(cmd, *aimbot.tick);

			auto weapon_type = weapon->GetCSWpnData()->WeaponType;
			auto weapon_id = weapon->GetItemDefinitionIndex();

			cmd->viewangles = ang;

			if (shots_fired > 1 && weapon_type != WEAPONTYPE_PISTOL && weapon_type != WEAPONTYPE_SNIPER_RIFLE && weapon_type != WEAPONTYPE_SHOTGUN)
				aimbot.NoRecoil();

			std::bernoulli_distribution distribution(0.1);

			// 10% chance a tick will be 80% speed
			//if (distribution(gen))
			//	frametime *= 0.8;

			float mult = 0.1f;

			switch (weapon_type)
			{
			case WEAPONTYPE_RIFLE:
				mult = (float)(settings.Get<int>("legit_rifle_speed"));
				break;
			case WEAPONTYPE_PISTOL:
				mult = (float)(settings.Get<int>("legit_pistol_speed"));
				break;
			case WEAPONTYPE_SNIPER_RIFLE:
				mult = (float)(settings.Get<int>("legit_sniper_speed"));
				break;
			case WEAPONTYPE_SUBMACHINEGUN:
				mult = (float)(settings.Get<int>("legit_smg_speed"));
				break;
			case WEAPONTYPE_SHOTGUN:
				mult = (float)(settings.Get<int>("legit_shotgun_speed"));
				break;
			case WEAPONTYPE_MACHINEGUN:
				mult = (float)(settings.Get<int>("legit_heavy_speed"));
				break;
			}

			//debug << mult << "\n";

			//mult = (float)(settings.Get<int>("legit_rifle_speed"));
			//mult = 0
			//mult = mult * 0.6f;

			//mult = 1.0f;

			float add = frametime * mult;

			if (smooth_time < 0.5f/*(float)(settings.Get<int>("legit_speed_max")) / 100.f*/)
			{
				//debug << "adding\n";

				smooth_time += add;

				smooth_time = std::clamp<float>(smooth_time, 0.f, 1.f);
			}
			//else
			//	ResetTime();

			/*
			if ((abs(real_ang.p - cmd->viewangles.p) + abs(real_ang.y - cmd->viewangles.y)) > 5)
			{
				std::uniform_real_distribution<> dis1(1.0f, 2.0f);
				std::uniform_real_distribution<> dis2(1.0f, 2.0f);

				cmd->viewangles.p += dis1(gen);
				cmd->viewangles.y += dis2(gen);
			}
			*/

			cmd->viewangles.p = lerp_axis(real_ang.p, cmd->viewangles.p, smooth_time);
			cmd->viewangles.y = lerp_axis(real_ang.y, cmd->viewangles.y, smooth_time);

			last_target = aimbot.target;
			last_shotsfired = shots_fired;
		}
		else
			ResetTime();
	}
	else
		ResetTime();

	//BaseFeature::SetArguments(CREATEMOVE, cmd, true, frametime);
}

void Legit::Destroy()
{

}

Legit legit;

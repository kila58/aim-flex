#include "strafe.hpp"

#include "../../aim-flex.hpp"

void Strafe::Init()
{

}

void Strafe::Invoke()
{
	auto cmd = GetArg<CUserCmd*>(GetArguments(), 0);

	C_BaseEntity* lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());
	if (!lp)
		return;

	if (!lp->IsOnGround())
	{
		Vector velocity = lp->GetVelocity();

		float angle_velocity = Rad2Deg(std::atan2(velocity.y, velocity.x));
		float angle_difference = std::remainderf(cmd->viewangles.y - angle_velocity, 360.f);

		float sidemax = cvar->FindVar("cl_sidespeed")->value<float>();

		cmd->move.y = angle_difference < 0.f ? sidemax : -sidemax;

		float gain = Rad2Deg(std::atan(cmd->move.y / cmd->move.x));
		static float gain_last = 0.f;

		if (!((gain_last < 0.f && gain < 0.f) || (gain_last > 0.f && gain > 0.f)))
			cmd->viewangles.y -= angle_difference;

		gain_last = gain;
	}
}

void Strafe::Destroy()
{

}

Strafe strafe;

#include "antiaim.hpp"

#include "../../aim-flex.hpp"

#include "../../features/aimbot/aimbot.hpp"

void AntiAim::Init()
{

}

void AntiAim::Invoke()
{
	auto cmd = GetArg<CUserCmd*>(GetArguments(CREATEMOVE), 0);

	cmd->viewangles.p = 89.f;

	bool isfake = cmd->command_number % 2;

	if (isfake)
	{
		cmd->viewangles.y -= 180.f;

		*aimbot.bsendpacket = false;

		fake = cmd->viewangles;
	}
	else
	{
		cmd->viewangles.y = 0.f;

		real = cmd->viewangles;
	}
}

void AntiAim::SetThirdPersonAngle()
{
	C_BaseEntity* lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());

	if (lp && lp->IsAlive() && cmdinput->m_fCameraInThirdPerson)
	{
		lp->SetViewAngle(fake);
	}
}
	
void AntiAim::Destroy()
{

}

AntiAim antiaim;

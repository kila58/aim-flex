#include "antiaim.hpp"

#include "../../aim-flex.hpp"

void AntiAim::Init()
{

}

void AntiAim::Invoke()
{
	auto cmd = GetArg<CUserCmd*>(GetArguments(CREATEMOVE), 0);

	cmd->viewangles.p = 89.f;
	cmd->viewangles.y -= 180.f;

	real = cmd->viewangles;
}

void AntiAim::SetThirdPersonAngle()
{
	C_BaseEntity* lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());

	if (lp && lp->IsAlive() && cmdinput->m_fCameraInThirdPerson)
	{
		lp->SetViewAngle(real);
	}
}
	
void AntiAim::Destroy()
{

}

AntiAim antiaim;

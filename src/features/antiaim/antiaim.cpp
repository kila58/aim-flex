#include "antiaim.hpp"

#include "../../aim-flex.hpp"

#include "../../features/aimbot/aimbot.hpp"
#include "../settings/settings.hpp"

void AntiAim::Init()
{

}

void AntiAim::Invoke()
{
	auto cmd = GetArg<CUserCmd*>(GetArguments(CREATEMOVE), 0);

	if (settings.Get<bool>("rage_antiaim_enabled"))
	{
		auto pitch = settings.Get<std::string>("rage_antiaim_pitch");

		if (pitch == "full down")
			cmd->viewangles.p = 89.f;
		else if (pitch == "half down")
			cmd->viewangles.p = 45.f;
		else if (pitch == "zero")
			cmd->viewangles.p = 0.f;

		bool isfake = cmd->command_number % 2;

		if (isfake)
		{
			auto fake_setting = settings.Get<std::string>("rage_antiaim_fake");

			if (fake_setting == "static")
				cmd->viewangles.y = 0.f;
			else if (fake_setting == "backwards")
				cmd->viewangles.y -= 180.f;

			*aimbot.bsendpacket = false;

			fake = cmd->viewangles;
		}
		else
		{
			auto real_setting = settings.Get<std::string>("rage_antiaim_real");

			if (real_setting == "static")
				cmd->viewangles.y = 0.f;
			else if (real_setting == "backwards")
				cmd->viewangles.y -= 180.f;

			real = cmd->viewangles;
		}
	}
	else
	{
		fake = cmd->viewangles;
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

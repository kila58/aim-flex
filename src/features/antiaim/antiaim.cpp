#include "antiaim.hpp"

#include "../../aim-flex.hpp"

#include "../../features/aimbot/aimbot.hpp"

#include <Random>

float randnumsick(float a, float b)
{
	static std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution(a, b);
	return distribution(generator);
}

int roundUp(int numToRound, int multiple)
{
	if (multiple == 0)
		return numToRound;

	int remainder = abs(numToRound) % multiple;
	if (remainder == 0)
		return numToRound;

	if (numToRound < 0)
		return -(abs(numToRound) - remainder);
	else
		return numToRound + multiple - remainder;
}

void AntiAim::Init()
{

}

int counter = 0;
constexpr int step = (180.f / 10.f);

void AntiAim::Invoke()
{
	auto cmd = GetArg<CUserCmd*>(GetArguments(CREATEMOVE), 0);

	cmd->viewangles.p = 89.f;

	bool isfake = cmd->command_number % 2;

	//*aimbot.bsendpacket = false;

	if (isfake)
	{
		counter += step;

		cmd->viewangles.y = counter;

		if (counter >= 180.f)
			counter = -180.f - step;
		
		//cvar->ConsoleColorPrintf("counter: " + std::to_string(counter) + "\n");

		*aimbot.bsendpacket = false;

		fake = cmd->viewangles;
	}
	else
	{
		cmd->viewangles.y = 0.f;//(float)roundUp(((int)counter - 180), 180.f);

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

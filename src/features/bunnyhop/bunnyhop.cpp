#include "bunnyhop.hpp"

#include "../../aim-flex.hpp"

#include "../../features/input/input.hpp"

void BunnyHop::Init()
{

}

void BunnyHop::Invoke()
{
	auto cmd = GetArg<CUserCmd*>(GetArguments(CREATEMOVE), 0);

	auto lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());

	if (lp && !lp->IsOnGround())
	{
		if (cmd->buttons & IN_JUMP)
			cmd->buttons &= ~IN_JUMP;
	}
}

void BunnyHop::Destroy()
{

}

BunnyHop bunnyhop;

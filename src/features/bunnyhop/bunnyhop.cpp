#include "bunnyhop.hpp"

#include "../../aim-flex.hpp"

#include "../../features/input/input.hpp"
#include "../../features/settings/settings.hpp"

void BunnyHop::Init()
{

}

void BunnyHop::Invoke()
{
	auto cmd = GetArg<CUserCmd*>(GetArguments(), 0);

	if (settings.Get<bool>("bhop_enabled"))
	{
		C_BaseEntity* lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());

		if (lp && !lp->IsOnGround())
		{
			if (cmd->buttons & IN_JUMP)
				cmd->buttons &= ~IN_JUMP;
		}
	}
}

void BunnyHop::Destroy()
{

}

BunnyHop bunnyhop;

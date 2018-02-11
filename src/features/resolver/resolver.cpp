#include "resolver.hpp"

#include "../../aim-flex.hpp"

#include "../../features/playermanager/playermanager.hpp"

void Resolver::Init()
{

}

void Resolver::Invoke()
{
	int stage = GetArg<int>(GetArguments(), 0);
	if (stage != FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		return;
}

void Resolver::RestoreEyeAngles()
{
	for (auto& target : playermanager.GetPlayers())
	{
		target.ent->SetEyeAngle(target.resolverinfo.eye);
	}
}

void Resolver::Destroy()
{

}

Resolver resolver;

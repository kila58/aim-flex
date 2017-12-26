#include "resolver.hpp"

#include "../../aim-flex.hpp"

#include "../../features/playermanager/playermanager.hpp"

void Resolver::Init()
{

}

void Resolver::Invoke()
{
	for (auto& target : playermanager.GetPlayers())
	{
		C_BaseEntity* p = target.ent;

		target.resolverinfo.lby = p->LowerBodyYaw();

		p->SetEyeAngle(Angle(p->GetEyeAngle().p, target.resolverinfo.lby, 0));
	}
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

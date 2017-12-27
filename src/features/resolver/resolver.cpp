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

	for (auto& target : playermanager.GetPlayers())
	{
		C_BaseEntity* p = target.ent;

		auto& info = target.resolverinfo;
		info.lby = p->LowerBodyYaw();
		
		// probably won't ever get updated in like nospread? add simulation check
		// add more "sources" of real info e.g. when people die etc
		// also add fakewalk check
		// also backtrack to head when shootin aim up
		// also add backtrack to lby breaker flick
		if (p->GetVelocity().Length2D() > 35.f/*0.1f*/ && p->IsOnGround())
		{
			auto& records = info.fakerecord.second;

			float difference = normalize(p->GetEyeAngle().y - info.lby);
			records.emplace_back(difference);

			if (records.size() == 8)
			{
				float average = std::accumulate(records.begin(), records.end(), 0.0f) / records.size();
				cvar->ConsoleColorPrintf(p->GetName() + ": " + std::to_string(average) + "\n");
				
				info.fakerecord.first = average > 5.f ? FAKE : REAL;

				records.clear();
			}
		};

		p->SetEyeAngle(Angle(p->GetEyeAngle().p, info.lby, 0));
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

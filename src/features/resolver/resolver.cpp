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
		// also add backtrack to hit

		//cvar->ConsoleColorPrintf("size: " + std::to_string(target.backtrackinfo.ticks.size()) + "\n");
		if (p->GetVelocity().Length2D() > 100.f/*0.1f*/ && p->IsOnGround())
		{
			auto& records = info.fakerecord.second;

			float difference = normalize(p->GetEyeAngle().y - info.lby);
			records.emplace_back(difference);

			// lower this and implement a system for filtering out "flukes"
			if (records.size() == 32)
			{
				float average = std::abs(std::accumulate(records.begin(), records.end(), 0.0f)) / records.size();

				if (average > 5.f)
				{
					cvar->ConsoleColorPrintf(p->GetName() + " average: " + std::to_string(average) + "\n");

					info.fakerecord.first = FAKE;
				}
				else
					info.fakerecord.first = REAL;

				records.clear();
			}

			/*
			if (records.size() == 16)
			{
				auto half = records.size() * 0.5;
				auto mid = records.begin() + half;

				float averagefirst = std::abs(std::accumulate(mid, records.end(), 0.0f) / half);
				float averagelast = std::abs(std::accumulate(records.begin(), mid, 0.0f) / half);
				
				float diff = target.backtrackinfo.ticks.back().time - target.backtrackinfo.ticks.front().time;
				//float avg = diff / target.backtrackinfo.ticks.size();

				cvar->ConsoleColorPrintf(std::to_string(TIME_TO_TICKS(diff)) + "\n");

				if (averagefirst > 5.f && averagelast > 5.f)
				{
					cvar->ConsoleColorPrintf(p->GetName() + " averagefirst: " + std::to_string(averagefirst) + "\n");
					cvar->ConsoleColorPrintf(p->GetName() + " averagelast: " + std::to_string(averagelast) + "\n");

					info.fakerecord.first = FAKE;
				}
				else
					info.fakerecord.first = REAL;

				records.clear();
			}
			*/
		};

		//p->SetEyeAngle(Angle(p->GetEyeAngle().p, clamp(normalize(p->GetEyeAngle().y), -180.f, 180.f), 0));
		//p->SetEyeAngle(Angle(-89.f, p->GetEyeAngle().y, 0));
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

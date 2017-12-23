#include "playermanager.hpp"

#include "../../aim-flex.hpp"

#include "../backtrack/backtrack.hpp"

bool InvalidPlayer(int i, C_BaseEntity* p, C_BaseEntity* lp)
{
	if (!p)
		return true;

	static player_info_t info;

	if (!engineclient->GetPlayerInfo(i, &info))
		return true;

	if (p == lp)
		return true;

	if (p->IsDormant())
		return true;

	if (!p->IsAlive())
		return true;

	if (p->GetTeam() == lp->GetTeam())
		return true;

	return false;
}

void PlayerManager::Init()
{

}

void PlayerManager::Invoke()
{
	C_BaseEntity* lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());
	Vector lporigin = lp->GetAbsOrigin();

	std::experimental::erase_if(players, [&lp](const Player& ply)
	{
		return InvalidPlayer(ply.index, ply.ent, lp);
	});

	for (int i = 1; i <= globals->maxClients; i++)
	{
		auto p = entitylist->GetClientEntity(i);

		if (!InvalidPlayer(i, p, lp))
		{
			static player_info_t info;
			if (!engineclient->GetPlayerInfo(i, &info))
				continue;

			// todo: maybe use Distance2DSqr? (it will produce massive numbers, needs benchmark)
			if (!PlayerExists(info.userID))
				players.emplace_back(i, info.userID, p, lporigin.Distance(p->GetAbsOrigin()));
		}
	}

	std::sort(players.begin(), players.end(), [](const Player& a, const Player& b)
	{
		return a.compare < b.compare;
	});
}

bool PlayerManager::PlayerExists(int uid)
{
	auto it = std::find(players.begin(), players.end(), uid);

	return (it != players.end());
}

void PlayerManager::RemovePlayer(int uid)
{
	std::experimental::erase_if(players, [&uid](const Player& ply)
	{
		return ply.userid == uid;
	});
}

std::deque<Player>& PlayerManager::GetPlayers()
{
	return players;
}

void PlayerManager::ClearPlayers()
{
	players.clear();
}

void PlayerManager::Destroy()
{

}

PlayerManager playermanager;

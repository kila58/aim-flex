#include "playermanager.hpp"

#include "../../aim-flex.hpp"

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
	players.clear();

	C_BaseEntity* lp = entitylist->GetClientEntity(engineclient->GetLocalPlayer());
	Vector lporigin = lp->GetAbsOrigin();

	for (int i = 1; i <= globals->maxClients; i++)
	{
		auto p = entitylist->GetClientEntity(i);

		// todo: maybe use Distance2DSqr? (it will produce massive numbers, needs benchmark)
		if (!InvalidPlayer(i, p, lp))
			players.emplace_back(p, lporigin.Distance(p->GetAbsOrigin()));
	}

	std::sort(players.begin(), players.end(), [](const Player& a, const Player& b)
	{
		return a.compare < b.compare;
	});
}

std::deque<Player>& PlayerManager::GetPlayers()
{
	return players;
}

void PlayerManager::Destroy()
{

}

PlayerManager playermanager;

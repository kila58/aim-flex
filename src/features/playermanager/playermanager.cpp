#include "playermanager.hpp"

#include "../../aim-flex.hpp"

#include "../backtrack/backtrack.hpp"

Player::Player(int index, int userid, C_BaseEntity* ent)
	: index(index), userid(userid), ent(ent), dormantplayer(playermanager.GetDormantPlayer(userid)) {};

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
	// FRAME_NET_UPDATE_POSTDATAUPDATE_START
	int stage = GetArg<int>(GetArguments(), 0);
	if (stage != FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		return;

	if (!populated)
		PopulateDormantPlayers();

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

			if (!PlayerExists(info.userID))
			{
				players.emplace_back(i, info.userID, p);
			}
		}
	}

	// todo: maybe use Distance2DSqr? (it will produce massive numbers, needs benchmark)
	for (auto& player : players)
	{
		player.compare = lporigin.Distance(player.ent->GetAbsOrigin());
		player.resolverinfo.absang = player.ent->GetAbsAngles();
		
		// if you ever need raw eye angle then add another var
		player.resolverinfo.eye = player.ent->GetEyeAngle();
		player.resolverinfo.eye = Angle(std::clamp(normalize(player.resolverinfo.eye.p), -89.f, 89.f), std::clamp(normalize(player.resolverinfo.eye.y), -180.f, 180.f), player.resolverinfo.eye.r);

		// todo: when rapid switching between teams with no delay random player becomes invalid for 1-2 ticks?
		if (!player.dormantplayer)
			player.dormantplayer = playermanager.GetDormantPlayer(player.userid);
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

static Player bad;
Player& PlayerManager::GetPlayer(int uid)
{
	if (players.empty())
		return bad;

	auto it = std::find(players.begin(), players.end(), uid);

	if (it == players.end())
		return bad;

	return players[std::distance(players.begin(), it)];
}

DormantPlayer* PlayerManager::GetDormantPlayer(int uid)
{
	if (dormantplayers.empty())
		return nullptr;

	auto it = std::find(dormantplayers.begin(), dormantplayers.end(), uid);

	if (it == dormantplayers.end())
		return nullptr;

	return &dormantplayers[std::distance(dormantplayers.begin(), it)];
}

bool PlayerManager::DormantPlayerExists(int uid)
{
	auto it = std::find(dormantplayers.begin(), dormantplayers.end(), uid);

	return (it != dormantplayers.end());
}

Player& PlayerManager::GetPlayer(C_BaseEntity* player)
{
	if (players.empty())
		return bad;

	int uid = player->GetUserID();
	if (!uid)
		return bad;

	auto it = std::find(players.begin(), players.end(), player->GetUserID());

	if (it == players.end())
		return bad;

	return players[std::distance(players.begin(), it)];
}

void PlayerManager::ClearPlayers()
{
	players.clear();
	dormantplayers.clear();
}

void PlayerManager::AddDormantPlayer(int uid)
{
	if (!DormantPlayerExists(uid))
		dormantplayers.emplace_back(uid);
}

void PlayerManager::RemoveDormantPlayer(int uid)
{
	std::experimental::erase_if(dormantplayers, [&uid](const DormantPlayer& ply)
	{
		return ply.userid == uid;
	});
}

void PlayerManager::ResetDormantPlayer(int uid)
{
	//auto dormantplayer = GetDormantPlayer(uid);
	//if (dormantplayer && dormantplayer->animstate)
	//	animations.ResetAnimationState(dormantplayer->animstate);
}

bool InvalidDormantPlayer(int i, C_BaseEntity* p, player_info_t& info)
{
	if (!p)
		return true;

	if (!engineclient->GetPlayerInfo(i, &info))
		return true;

	if (info.ishltv)
		return true;

	return false;
}

void PlayerManager::PopulateDormantPlayers()
{
	for (int i = 1; i <= globals->maxClients; i++)
	{
		auto p = entitylist->GetClientEntity(i);

		static player_info_t info;
		if (!InvalidDormantPlayer(i, p, info))
			AddDormantPlayer(info.userID);
	}

	SetPopulated(true);
}

void PlayerManager::SetPopulated(bool setpopulated)
{
	populated = setpopulated;
}

void PlayerManager::Destroy()
{

}

PlayerManager playermanager;

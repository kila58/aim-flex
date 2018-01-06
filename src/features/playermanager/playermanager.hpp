#pragma once

#include <array>

#include "../features.hpp"

#include "../backtrack/backtrack.hpp"
#include "../resolver/resolver.hpp"

#include "../../sdk/IPlayerAnimState.hpp"
#include "../../sdk/CUtlVector.hpp"

class C_BaseEntity;

struct Player
{
public:
	Player() : index(-1) {}
	Player(int index, int userid, C_BaseEntity* ent) : index(index), userid(userid), ent(ent) {}

	friend bool operator==(const Player& first, int userid) { return first.userid == userid; }
	friend bool operator==(int userid, const Player& second) { return userid == second.userid; }
	friend bool operator==(const Player& first, const Player& second) { return first.userid == second.userid; }
	operator bool() const { return index != -1; }

	int index;
	int userid;
	C_BaseEntity* ent;
	float compare;
	BacktrackInfo backtrackinfo;
	ResolverInfo resolverinfo;
	CUtlVector<C_AnimationLayer> animationlayers;
	float poses[24];
};

class PlayerManager : public BaseFeature
{
private:
	std::deque<Player> players;
public:
	PlayerManager() : BaseFeature(FRAMESTAGENOTIFY, 1u) {}

	void Init();
	void Invoke();
	bool PlayerExists(int uid);
	void RemovePlayer(int uid);
	std::deque<Player>& GetPlayers();
	Player& GetPlayer(int uid);
	Player& GetPlayer(C_BaseEntity* player);
	void ClearPlayers();
	void Destroy();
};

extern PlayerManager playermanager;

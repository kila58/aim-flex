#pragma once

#include "../../aim-flex.hpp"

#include "../features.hpp"

#include "../backtrack/backtrack.hpp"
#include "../resolver/resolver.hpp"

class C_BaseEntity;

// todo: fix this
struct AnimStateDeleter 
{
	void operator()(CCSGOPlayerAnimState* animstate)
	{
		//g_pMemAlloc->Free(animstate);
	}
};

struct DormantPlayer
{
public:
	DormantPlayer(int userid) : userid(userid) {}

	friend bool operator==(int userid, const DormantPlayer& second) { return userid == second.userid; }
	friend bool operator==(const DormantPlayer& first, const DormantPlayer& second) { return first.userid == second.userid; }

	int userid;
	std::unique_ptr<CCSGOPlayerAnimState, AnimStateDeleter> animstate;
};

struct Player
{
public:
	Player() : index(-1) {}
	Player(int index, int userid, C_BaseEntity* ent);

	friend bool operator==(const Player& first, int userid) { return first.userid == userid; }
	friend bool operator==(int userid, const Player& second) { return userid == second.userid; }
	friend bool operator==(const Player& first, const Player& second) { return first.userid == second.userid; }
	operator bool() const { return index != -1 && dormantplayer; }

	int index;
	int userid;
	C_BaseEntity* ent;
	float compare;
	BacktrackInfo backtrackinfo;
	ResolverInfo resolverinfo;
	AnimLayerVec animationlayers;
	PoseArray poses;
	DormantPlayer* dormantplayer = nullptr;
};

class PlayerManager : public BaseFeature
{
private:
	std::deque<Player> players;
	std::deque<DormantPlayer> dormantplayers;
	bool populated = false;
public:
	PlayerManager() : BaseFeature(FRAMESTAGENOTIFY, 1u) {}

	void Init();
	void Invoke();
	bool PlayerExists(int uid);
	void RemovePlayer(int uid);
	std::deque<Player>& GetPlayers();
	Player& GetPlayer(int uid);
	DormantPlayer* GetDormantPlayer(int uid);
	bool DormantPlayerExists(int uid);
	Player& GetPlayer(C_BaseEntity* player);
	void ClearPlayers();
	DormantPlayer* AddDormantPlayer(int uid);
	void RemoveDormantPlayer(int uid);
	void ResetDormantPlayer(int uid);
	void PopulateDormantPlayers();
	void SetPopulated(bool setpopulated);
	void Destroy();
};

extern PlayerManager playermanager;

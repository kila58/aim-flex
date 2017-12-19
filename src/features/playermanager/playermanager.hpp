#pragma once

#include "../features.hpp"

class C_BaseEntity;

struct Player
{
public:
	Player(C_BaseEntity* ent, float compare) : ent(ent), compare(compare) {}

	C_BaseEntity* ent;
	float compare;
};

class PlayerManager : public BaseFeature
{
private:
	std::deque<Player> players;
public:
	PlayerManager() : BaseFeature(CREATEMOVE, 2u) {}

	void Init();
	void Invoke();
	std::deque<Player>& GetPlayers();
	void Destroy();
};

extern PlayerManager playermanager;

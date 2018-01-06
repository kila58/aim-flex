#pragma once

#include "../features.hpp"

#include "../../angle.hpp"
#include "../../vector.hpp"

class CUserCmd;
class C_BaseEntity;

struct Tick
{
	Tick() {}
	Tick(float time, float curtime, Vector head, int tickcount) : time(time), curtime(curtime), head(head), tickcount(tickcount) {}

	friend bool operator==(const Tick& first, const Tick& second) { return first.time == second.time; }
	friend bool operator==(const Tick& first, float sim) { return first.time == sim; }
	operator bool() const { return curtime != 0.f; }

	float curtime = 0.f;
	float time;
	int tickcount;
	Vector head;
};

class BacktrackInfo
{
public:
	std::deque<Tick> ticks;

	Tick& FindTick(C_BaseEntity* p, float simulation);
};

class Backtrack : public BaseFeature
{
public:
	Backtrack() : BaseFeature(CREATEMOVE, 3u) {}

	void Init();
	float GetLerpTime();
	bool TickIsValid(const Tick& tick, float lerp);
	void Invoke();
	void BacktrackToTick(CUserCmd* cmd, const Tick& tick);
	void Destroy();
};

extern Backtrack backtrack;

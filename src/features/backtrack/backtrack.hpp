#pragma once

#include "../features.hpp"

#include "../../angle.hpp"
#include "../../vector.hpp"

class CUserCmd;

struct Tick
{
	Tick(float time, Vector head) : time(time), head(head) {}

	friend bool operator==(const Tick& first, const Tick& second) { return first.time == second.time; }

	float time;
	Vector head;
};

struct BacktrackInfo
{
	std::deque<Tick> ticks;
};

class Backtrack : public BaseFeature
{
public:
	Backtrack() : BaseFeature(FRAMESTAGENOTIFY, 2u) {}

	void Init();
	float GetServerTickCount();
	float GetLerpTime();
	void Invoke();
	void BacktrackToTick(CUserCmd* cmd, const Tick& tick);
	void Destroy();
};

extern Backtrack backtrack;

#pragma once

#include "../../aim-flex.hpp"

#include "../features.hpp"

struct HitboxInfo
{
	Vector head;
	Vector mins;
	Vector maxs;
	float radius;
};

struct Tick
{
	Tick() {}
	Tick(float time, HitboxInfo hitboxinfo)
		: time(time), hitboxinfo(hitboxinfo), tickcount(globals->tickcount) {}

	friend bool operator==(const Tick& first, const Tick& second) { return first.time == second.time; }
	friend bool operator==(const Tick& first, float sim) { return first.time == sim; }
	operator bool() const { return time != 0.f; }

	float time = 0.f;
	int tickcount;
	HitboxInfo hitboxinfo;
};

class BacktrackInfo
{
public:
	std::deque<Tick> ticks;

	Tick& FindTick(C_BaseEntity* p, float simulation);
};

class Animations
{
public:
	void CreateAnimationState(CCSGOPlayerAnimState* state, C_BaseEntity* player);
	void UpdateAnimationState(CCSGOPlayerAnimState* state, Angle ang);
	void UpdateServerAnimations();
	void PVSFix();
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
extern Animations animations;

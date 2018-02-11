#pragma once

#include "../../aim-flex.hpp"

#include "../features.hpp"

struct HitboxInfo
{
	Vector head;
	Vector mins;
	Vector maxs;
	Vector minsnoradius;
	Vector maxsnoradius;
	float radius;
	int hitbox;
};

struct Tick
{
	Tick() {}
	Tick(float time, HitboxInfo hitboxinfo, Vector absorigin, Angle absangle)
		: time(time), hitboxinfo(hitboxinfo), tickcount(globals->tickcount), absorigin(absorigin), absangle(absangle) {}

	friend bool operator==(const Tick& first, const Tick& second) { return first.time == second.time; }
	friend bool operator==(const Tick& first, float sim) { return first.time == sim; }
	operator bool() const { return time != 0.f; }

	float time = 0.f;
	int tickcount;
	Vector absorigin;
	Angle absangle;
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
	void BulletImpact(Vector pos, C_BaseEntity* player);
	void Destroy();
};

extern Backtrack backtrack;
extern Animations animations;

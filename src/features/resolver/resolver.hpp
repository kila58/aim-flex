#pragma once

#include "../features.hpp"

#include "../../angle.hpp"
#include "../../vector.hpp"

enum FakeType
{
	UNDETERMINED = -1,
	REAL = 0,
	FAKE = 1
};

typedef std::pair<FakeType, std::deque<float>> FakeRecord;

struct ResolverInfo
{
	ResolverInfo() { fakerecord.first = UNDETERMINED; }

	FakeRecord fakerecord;
	Angle eye;
	Angle absang;
	float lby = 0.f;
};

class Resolver : public BaseFeature
{
public:
	Resolver() : BaseFeature(FRAMESTAGENOTIFY, 2u) {}

	void Init();
	void Invoke();
	void RestoreEyeAngles();
	void Destroy();
};

extern Resolver resolver;

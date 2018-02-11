#pragma once

#include "../features.hpp"

#include "../../angle.hpp"
#include "../../vector.hpp"

struct ResolverInfo
{
	Angle eye;
	Angle absang;
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

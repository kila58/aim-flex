#pragma once

#include "../features.hpp"

#include "../../angle.hpp"
#include "../../vector.hpp"

struct ResolverInfo
{
	Angle eye;
	float lby;
};

class Resolver : public BaseFeature
{
public:
	Resolver() : BaseFeature() {}

	void Init();
	void Invoke();
	void RestoreEyeAngles();
	void Destroy();
};

extern Resolver resolver;

#pragma once

#include "../features.hpp"

#include "../../angle.hpp"
#include "../../vector.hpp"

class C_BaseEntity;
class CUserCmd;
struct Tick;

class Rage : public BaseFeature
{
public:
	Rage() : BaseFeature() {}

	void Init();
	bool FindTarget(CUserCmd* cmd, Angle& ang);
	void Invoke();
	void Destroy();
};

extern Rage rage;
